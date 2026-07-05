# Raspberry Pi Pico 1 with QEMU

This repository is a step-by-step tutorial for running Raspberry Pi Pico 1
firmware with a QEMU machine model for the RP2040.

The goal is not to replace the official Raspberry Pi Pico documentation. The
goal is to show how the usual Pico SDK workflow can be redirected to QEMU so
that small firmware examples can be built, run, observed, and tested without a
physical board.

The tutorial currently assumes a Linux host, preferably Debian or Ubuntu.

## Get the Tutorial

This tutorial is meant to be used as a Git repository. Clone it under
`$HOME/src`, or replace `$HOME` with another base directory and keep that choice
throughout the tutorial.

```sh
mkdir -p "$HOME/src"
cd "$HOME/src"
git clone https://github.com/2xs/qemu-rp2040-pico-tutorial.git
cd qemu-rp2040-pico-tutorial
```

The commands in the following steps assume that the tutorial repository is
available at:

```text
$HOME/src/qemu-rp2040-pico-tutorial
```

## What You Will Learn

### Step 1: Install `qemu-pipico1`

Build a QEMU branch that contains the experimental `raspi-pico` machine, then
run a first command to verify that QEMU recognizes the Raspberry Pi Pico 1
target.

See [01-install-qemu-pipico1](01-install-qemu-pipico1/README.md).

### Step 2: Install the Raspberry Pi Pico SDK

Install the Arm embedded toolchain, clone the Raspberry Pi Pico SDK and the
official examples, and prepare the environment variables used by CMake.

See [02-install-pico-sdk](02-install-pico-sdk/README.md).

### Step 3: Build and Run `hello_world` on UART0

Compile the Pico SDK `hello_world/serial` example and run it on QEMU through
UART0 in a terminal-only QEMU session.

See [03-hello-world-uart0](03-hello-world-uart0/README.md).

### Step 4: Build and Run `hello_world` from Core 1

Compile a small SDK-based multicore example where core 0 starts core 1 and
core 1 prints a message through the UART path.

See [04-hello-world-core1](04-hello-world-core1/README.md).

### Step 5: Build and Run `reboot_counter`

Compile and run a small firmware that stores a counter in the emulated external
flash. This demonstrates how `flash-file=...` can preserve flash contents
between QEMU runs.

See [05-reboot-counter-flash](05-reboot-counter-flash/README.md).

### Step 6: Read the Flash Unique Board ID

Compile a small SDK-based example derived from `system/unique_board_id`, then
run it with QEMU's default flash unique ID and with an explicit `flash-uid=...`
machine option.

See [06-board-unique-id](06-board-unique-id/README.md).

## Repository Layout

Each tutorial step lives in its own directory:

```text
01-install-qemu-pipico1/
02-install-pico-sdk/
03-hello-world-uart0/
04-hello-world-core1/
05-reboot-counter-flash/
06-board-unique-id/
```

Every step has a `README.md` explaining:

- what the reader will learn;
- which packages or files are needed;
- the commands to run;
- the expected output;
- common failure modes.

When a step needs shell scripts, C sources, CMake files, or helper tools, those
files live next to the step README.

## Current Status

The `raspi-pico` QEMU machine used by this tutorial is not part of stock QEMU
yet. Step 1 therefore builds QEMU from an experimental branch:

```text
https://github.com/2xs/qemu-rp2040-pico.git
```

Once the machine is upstream, Step 1 can be simplified to use a normal QEMU
release or distribution package.
