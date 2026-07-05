# Step 6: Read the Flash Unique Board ID

In this step you will run firmware based on the official Pico SDK
`system/unique_board_id` example.

On an RP2040 board, the board identifier is normally read from the external
flash chip. QEMU models this value and exposes it through the same Pico SDK API:
`pico_get_unique_board_id()`.

## What This Step Teaches

You will learn how to:

- build a small program that reads the Pico board unique ID through the SDK;
- observe QEMU's default emulated flash unique ID;
- override that value with the `flash-uid=...` machine option;
- understand that this ID is an emulated flash property, not data stored in
  the raw `flash-file=...` image.

## Assumptions

This step assumes that you completed:

- [Step 1](../01-install-qemu-pipico1/README.md), so QEMU is available at
  `$HOME/src/qemu-rp2040-pico/build/qemu-system-arm`;
- [Step 2](../02-install-pico-sdk/README.md), so `PICO_SDK_PATH` is set;
- [Step 3](../03-hello-world-uart0/README.md), so you are comfortable running
  an ELF image with `-kernel`.

The tutorial repository is expected at:

```text
$HOME/src/qemu-rp2040-pico-tutorial
```

## Build the Example

Copy the tutorial version of the SDK unique-board-ID example next to your other
Pico projects:

```sh
cp -R "$HOME/src/qemu-rp2040-pico-tutorial/06-board-unique-id/unique-board-id" \
  "$HOME/src/unique-board-id"
cd "$HOME/src/unique-board-id"
```

Build it:

```sh
cmake -G Ninja -DPICO_BOARD=pico -S . -B build
cmake --build build
```

If CMake tries to download `picotool` and your machine has no network access,
configure the build without the UF2 post-processing step:

```sh
cmake -G Ninja -DPICO_BOARD=pico -DPICO_NO_PICOTOOL=1 -S . -B build
cmake --build build
```

Expected output files include:

```text
build/unique_board_id.elf
```

The C code is derived from Raspberry Pi's `pico-examples/system/unique_board_id`
example. The main tutorial-specific change is in `CMakeLists.txt`: UART stdio
is enabled and USB stdio is disabled, so the output appears directly in QEMU's
terminal.

## Run with QEMU's Default Flash UID

Run the firmware:

```sh
"$HOME/src/qemu-rp2040-pico/build/qemu-system-arm" \
  -machine raspi-pico \
  -kernel build/unique_board_id.elf \
  -nographic
```

Expected output:

```text
Unique identifier: 3e b8 a7 49 3f cc 06 08
```

This is QEMU's default emulated flash unique ID for the `raspi-pico` machine.
It is stable so that repeated runs of the same command behave like the same
emulated board.

The program then returns from `main`, and QEMU exits cleanly.

## Override the Flash UID

Run the same firmware again, this time with `flash-uid=0011223344556677`:

```sh
"$HOME/src/qemu-rp2040-pico/build/qemu-system-arm" \
  -machine raspi-pico,flash-uid=0011223344556677 \
  -kernel build/unique_board_id.elf \
  -nographic
```

Expected output:

```text
Unique identifier: 00 11 22 33 44 55 66 77
```

The `flash-uid` value must contain exactly 16 hexadecimal digits, representing
the 8 bytes returned by the emulated flash unique-ID command.

## Relationship with `flash-file`

The unique board ID belongs to the emulated flash device, but QEMU does not
store it inside the raw `flash-file=...` contents. This keeps `flash-file`
simple: it remains a raw image of the addressable flash bytes.

If you need a specific stable board ID for a test, pass it explicitly:

```sh
"$HOME/src/qemu-rp2040-pico/build/qemu-system-arm" \
  -machine raspi-pico,flash-file=flash.bin,flash-uid=0011223344556677 \
  -kernel build/unique_board_id.elf \
  -nographic
```

Congratulations! You have now read the same board-ID API that Pico SDK code
uses on real RP2040 boards, and you have controlled the emulated value from the
QEMU command line.

## Troubleshooting

### QEMU Prints Nothing

Check that you copied and built the tutorial project, not the unmodified SDK
example:

```sh
grep pico_enable_stdio_uart CMakeLists.txt
```

The tutorial project enables UART stdio explicitly. The stock SDK example may
not do that, depending on your SDK configuration.

### QEMU Rejects `flash-uid`

The value must be exactly 16 hexadecimal digits:

```text
flash-uid=0011223344556677
```

Do not add `0x`, spaces, or separators.
