# Step 3: Build and Run `hello_world` on UART0

In this step you will build the official Pico SDK UART hello-world example and
run it on the QEMU Raspberry Pi Pico 1 machine.

By the end of the step you will see a message printed by firmware running on
the emulated RP2040 UART0.

## What This Step Teaches

You will learn how to:

- build the SDK `hello_world/serial` example;
- run a Pico 1 ELF firmware image with QEMU;
- run the same firmware as a UF2 image;
- use the QEMU terminal as the emulated UART0 console;
- stop QEMU cleanly from the terminal.

## Assumptions

This step assumes that you completed:

- [Step 1](../01-install-qemu-pipico1/README.md), so QEMU is available at
  `$HOME/src/qemu-rp2040-pico/build/qemu-system-arm`;
- [Step 2](../02-install-pico-sdk/README.md), so `pico-sdk`,
  `pico-examples`, and `PICO_SDK_PATH` are available.

The commands below use:

```text
$HOME/src/qemu-rp2040-pico
$HOME/src/pico-sdk
$HOME/src/pico-examples
```

If you used another base directory, replace `$HOME/src` consistently.

## Check the Environment

Verify that the SDK path is still set:

```sh
echo "$PICO_SDK_PATH"
```

Expected shape:

```text
/home/your-user/src/pico-sdk
```

Verify that the QEMU binary from Step 1 is available:

```sh
"$HOME/src/qemu-rp2040-pico/build/qemu-system-arm" --version
```

## Locate the SDK UART Hello World

Go back to the `pico-examples` build directory created in Step 2:

```sh
cd "$HOME/src/pico-examples/build"
```

Check that the UART hello-world firmware files are still there:

```sh
ls hello_world/serial/hello_serial.elf \
   hello_world/serial/hello_serial.uf2
```

Expected output:

```text
hello_world/serial/hello_serial.elf
hello_world/serial/hello_serial.uf2
```

If these files are missing, rebuild the target:

```sh
ninja hello_serial
```

## Run the ELF Image

Run the ELF firmware directly:

```sh
"$HOME/src/qemu-rp2040-pico/build/qemu-system-arm" \
  -machine raspi-pico \
  -kernel hello_world/serial/hello_serial.elf \
  -nographic
```

The Raspberry Pi Pico 1 has no display device. Here `-nographic` keeps QEMU in
the terminal and connects the emulated UART0 console to that terminal.

Expected output:

```text
Hello, world!
```

The exact capitalization and punctuation come from the SDK example and may
change if Raspberry Pi updates `pico-examples`.

Stop QEMU with:

```text
Ctrl-a x
```

Press `Ctrl-a`, release both keys, then press `x`.

## Run the UF2 Image

The Pico SDK also produces a UF2 file, which is the format commonly copied to a
physical Pico in BOOTSEL mode.

Run the UF2 firmware with QEMU:

```sh
"$HOME/src/qemu-rp2040-pico/build/qemu-system-arm" \
  -machine raspi-pico \
  -kernel hello_world/serial/hello_serial.uf2 \
  -nographic
```

Expected output is the same:

```text
Hello, world!
```

For this QEMU machine, `-kernel` can load either an ELF image or a UF2 image.
At this stage, think of `-kernel` as asking QEMU to prepare a fresh emulated
flash image from that firmware before starting the Pico 1. If you run QEMU
again with the same `-kernel` argument, the firmware image is loaded again.

No state is kept between two QEMU executions in this mode. Persistent storage
is available through a separate QEMU option, where a host file represents the
Pico 1 external flash. That workflow is introduced in Step 5.

## Run Your First Local Project

If you built `my-first-pico-code` in Step 2, you can run it the same way:

```sh
"$HOME/src/qemu-rp2040-pico/build/qemu-system-arm" \
  -machine raspi-pico \
  -kernel "$HOME/src/my-first-pico-code/build/my_first_pico_code.elf" \
  -nographic
```

Expected output:

```text
my own hello world
```

Congratulations! You can now build Pico 1 firmware and run it on QEMU through
UART0.

Next step: start a second RP2040 core and print from core 1. Go to
[`04-hello-world-core1`](../04-hello-world-core1/README.md).

## Troubleshooting

### QEMU Prints Nothing

Check that you passed a firmware image with `-kernel` and that you are using
the `serial` hello-world example, not the USB one:

```sh
ls "$HOME/src/pico-examples/build/hello_world/serial/hello_serial.elf"
```

The `hello_world/usb` example is not the UART example used in this step.

### QEMU Does Not Exit with `Ctrl-C`

With `-nographic`, `Ctrl-C` is sent to the guest. Use the QEMU escape sequence
instead:

```text
Ctrl-a x
```

### `raspi-pico` Is Not a Supported Machine

Check that you are running the QEMU binary built in Step 1, not the system QEMU
installed by your Linux distribution:

```sh
"$HOME/src/qemu-rp2040-pico/build/qemu-system-arm" -machine help | grep raspi-pico
```

### CMake or Ninja Cannot Find the SDK

Return to Step 2 and check `PICO_SDK_PATH`:

```sh
echo "$PICO_SDK_PATH"
```

If it is empty, run:

```sh
export PICO_SDK_PATH="$HOME/src/pico-sdk"
```
