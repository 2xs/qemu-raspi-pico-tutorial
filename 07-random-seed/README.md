# Step 7: Control the Emulated Random Source

In this step you will run firmware based on the Pico SDK `pico_rand` API.

The RP2040 SDK can use the ring oscillator random bit as an entropy source.
QEMU models that ROSC random bit. By default it is seeded from QEMU's host
random source, so two QEMU runs normally produce different values. For
repeatable tests, the `raspi-pico` machine also provides a
`rosc-random-seed=...` option.

## What This Step Teaches

You will learn how to:

- build a small program that calls the SDK `get_rand_64()` function;
- observe non-reproducible output with QEMU's default random source;
- force a deterministic ROSC random stream with `rosc-random-seed=...`;
- understand why deterministic random streams are useful in CI tests.

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

Copy the tutorial random example next to your other Pico projects:

```sh
cp -R "$HOME/src/qemu-rp2040-pico-tutorial/07-random-seed/rng-eight-bytes" \
  "$HOME/src/rng-eight-bytes"
cd "$HOME/src/rng-eight-bytes"
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
build/rng_eight_bytes.elf
```

The code uses the SDK `pico_rand` library and calls `get_rand_64()`. It prints
the returned value as 8 bytes.

For this tutorial, the project uses the SDK ROSC seed source and disables the
time, bus-counter, board-ID, RAM-hash and per-call ROSC entropy sources. That
keeps the demonstration focused on QEMU's ROSC random-bit model during PRNG
initialization, so `rosc-random-seed=...` fully controls the sequence.

## Run without a Fixed Seed

Run the firmware once:

```sh
"$HOME/src/qemu-rp2040-pico/build/qemu-system-arm" \
  -machine raspi-pico \
  -kernel build/rng_eight_bytes.elf \
  -nographic
```

Example output:

```text
Random bytes: 5d 04 cb 49 00 19 8e 51
```

Run the same command again:

```sh
"$HOME/src/qemu-rp2040-pico/build/qemu-system-arm" \
  -machine raspi-pico \
  -kernel build/rng_eight_bytes.elf \
  -nographic
```

Expected result: the printed bytes should normally be different, because QEMU
seeds the emulated ROSC random source from the host random source.

## Run with a Fixed Seed

Now run the firmware with a deterministic ROSC seed:

```sh
"$HOME/src/qemu-rp2040-pico/build/qemu-system-arm" \
  -machine raspi-pico,rosc-random-seed=0x1234 \
  -kernel build/rng_eight_bytes.elf \
  -nographic
```

Run the same command a second time:

```sh
"$HOME/src/qemu-rp2040-pico/build/qemu-system-arm" \
  -machine raspi-pico,rosc-random-seed=0x1234 \
  -kernel build/rng_eight_bytes.elf \
  -nographic
```

Expected result: both runs print the same 8 bytes. The exact byte values depend
on the QEMU implementation, but the important property is reproducibility when
the same seed is used.

With the QEMU branch used by this tutorial, `rosc-random-seed=0x1234` prints:

```text
Random bytes: fb 58 9e 68 ef 4a 60 d7
```

Try another seed:

```sh
"$HOME/src/qemu-rp2040-pico/build/qemu-system-arm" \
  -machine raspi-pico,rosc-random-seed=0x5678 \
  -kernel build/rng_eight_bytes.elf \
  -nographic
```

Expected result: this prints a different deterministic sequence.

Congratulations! You have now used the SDK random API with both QEMU's default
host-seeded entropy and a reproducible emulated ROSC random stream.

## Troubleshooting

### The Seeded Runs Are Not Identical

Check that you copied the tutorial project and not the full SDK
`system/rand` example:

```sh
grep PICO_RAND_ENTROPY_SRC_TIME CMakeLists.txt
```

The full SDK example deliberately mixes more entropy sources. This tutorial
project disables them so `rosc-random-seed=...` controls the output.

### QEMU Rejects `rosc-random-seed`

Use an integer value accepted by QEMU, such as:

```text
rosc-random-seed=0x1234
```
