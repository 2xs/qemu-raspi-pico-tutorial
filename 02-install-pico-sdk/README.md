# Step 2: Install the Raspberry Pi Pico SDK

In this step you will install the tools needed to build Raspberry Pi Pico 1
firmware with the official C/C++ SDK.

By the end of the step you will be able to configure the official
`pico-examples` tree with CMake and generate build files for the Pico 1 board.

## What This Step Teaches

You will learn how to:

- install the Debian/Ubuntu packages needed for Pico SDK development;
- clone the official `pico-sdk` repository;
- clone the official `pico-examples` repository;
- expose the SDK location through `PICO_SDK_PATH`;
- run a first CMake configuration for the Pico 1 target.

## Assumptions

This step continues the layout used in Step 1:

```sh
mkdir -p "$HOME/src"
cd "$HOME/src"
```

The QEMU binary built in Step 1 is expected at:

```text
$HOME/src/qemu-rp2040-pico/build/qemu-system-arm
```

This step targets Debian or Ubuntu. The commands below avoid IDE-specific
setup and keep the workflow command-line based.

## Install Pico Build Dependencies

Install the Arm embedded compiler, CMake, and the small set of tools used by
the SDK examples:

```sh
sudo apt update
sudo apt install --no-install-recommends -y \
  build-essential \
  ca-certificates \
  cmake \
  gcc-arm-none-eabi \
  git \
  libnewlib-arm-none-eabi \
  libstdc++-arm-none-eabi-newlib \
  ninja-build \
  python3
```

The package names above are the usual Debian/Ubuntu packages for command-line
Pico SDK development. If your distribution splits the Arm C++ runtime
differently, install the equivalent `arm-none-eabi` C and C++ runtime packages.

## Clone the Pico SDK

Clone the official SDK with its submodules:

```sh
cd "$HOME/src"
git clone https://github.com/raspberrypi/pico-sdk.git
cd pico-sdk
git submodule update --init
```

The SDK is maintained by Raspberry Pi and developed independently from this
tutorial and from the QEMU branch used here. For repeatable local experiments,
you may later choose to pin it to a known tag, but this tutorial starts with
the default branch.

## Export `PICO_SDK_PATH`

Tell CMake where the SDK lives:

```sh
export PICO_SDK_PATH="$HOME/src/pico-sdk"
```

To make this persistent for future shells, add the same line to your shell
startup file:

```sh
echo 'export PICO_SDK_PATH="$HOME/src/pico-sdk"' >> "$HOME/.bashrc"
```

For the current terminal, keep the explicit `export` command above. The next
steps assume `PICO_SDK_PATH` is set.

## Clone the Official Examples

Clone the official example programs:

```sh
cd "$HOME/src"
git clone https://github.com/raspberrypi/pico-examples.git
```

This tutorial will reuse the official examples where possible, then introduce
small focused examples when QEMU-specific behavior needs to be demonstrated.

## Configure the Examples

Configure the examples for the original Raspberry Pi Pico board:

```sh
cd "$HOME/src/pico-examples"
mkdir -p build
cd build
cmake -G Ninja -DPICO_BOARD=pico ..
```

The `PICO_BOARD=pico` setting selects the Raspberry Pi Pico 1 board definition,
which is the board modelled by QEMU's `raspi-pico` machine in this tutorial.

## Verify the SDK Setup

After CMake finishes, build one small target that later steps will use:

```sh
ninja hello_serial
```

Expected output files include:

```text
hello_world/serial/hello_serial.elf
hello_world/serial/hello_serial.uf2
```

Verify that the ELF exists:

```sh
ls hello_world/serial/hello_serial.elf
```

If the file is present, the SDK, toolchain, and example tree are ready.

## What About Your Own Code?

The official examples are useful, but the goal is to compile your own Pico 1
firmware too. This step provides a minimal SDK project in:

```text
my-first-pico-code/
```

It contains:

```text
my-first-pico-code/
├── CMakeLists.txt
└── main.c
```

The C file prints a small message on the UART path:

```c
puts("my own hello world");
```

Copy this project next to the SDK and examples:

```sh
cp -R "$HOME/src/qemu-rp2040-pico-tutorial/02-install-pico-sdk/my-first-pico-code" \
  "$HOME/src/my-first-pico-code"
cd "$HOME/src/my-first-pico-code"
```

Then configure and build it in two commands:

```sh
cmake -G Ninja -DPICO_BOARD=pico -S . -B build
cmake --build build
```

Expected output files include:

```text
build/my_first_pico_code.elf
build/my_first_pico_code.uf2
```

The next step will show how to run this kind of ELF or UF2 image with QEMU.

Congratulations! You are now able to compile Raspberry Pi Pico 1 software.

Next step: run the SDK `hello_world` UART example with QEMU. Go to
[`03-hello-world-uart0`](../03-hello-world-uart0/README.md).

## Troubleshooting

### CMake cannot find the Pico SDK

Check that `PICO_SDK_PATH` is set:

```sh
echo "$PICO_SDK_PATH"
```

It should print:

```text
/home/your-user/src/pico-sdk
```

If it is empty, run:

```sh
export PICO_SDK_PATH="$HOME/src/pico-sdk"
```

Then retry CMake from a clean build directory.

### `arm-none-eabi-gcc` is not found

Install the cross compiler:

```sh
sudo apt install gcc-arm-none-eabi
```

Then verify it:

```sh
arm-none-eabi-gcc --version
```

### `ninja` is not found

Install:

```sh
sudo apt install ninja-build
```

### The build runs out of disk space

The full `pico-examples` build tree can grow quickly. This tutorial only needs
a few small examples, but CMake still prepares the whole example tree. Free
some disk space or build in a larger directory, then rerun the CMake command.
