# Step 1: Install `qemu-pipico1`

In this step you will build a QEMU tree that contains experimental support for
the Raspberry Pi Pico 1 / RP2040 machine.

By the end of the step you will be able to run:

```sh
qemu-system-arm -machine raspi-pico -machine help
```

and confirm that QEMU knows about the `raspi-pico` machine.

## What This Step Teaches

You will learn how to:

- install the Debian/Ubuntu packages needed to build QEMU;
- clone the QEMU branch used by this tutorial;
- configure QEMU for the `arm-softmmu` target only;
- build `qemu-system-arm`;
- verify that the `raspi-pico` machine is available;
- keep the resulting QEMU binary in a predictable location for later steps.

## Assumptions

This step targets Debian or Ubuntu. The commands below have the shape expected
on a recent Ubuntu LTS or Debian stable system.

The tutorial uses a working directory under `$HOME/src`:

```sh
mkdir -p "$HOME/src"
cd "$HOME/src"
```

You may choose another directory, but later steps assume this layout unless you
adapt the paths.

## Install Build Dependencies

Install the minimal packages needed to configure and build QEMU:

```sh
sudo apt update
sudo apt install --no-install-recommends -y \
  bash \
  bc \
  bison \
  bzip2 \
  ca-certificates \
  ccache \
  flex \
  gcc \
  git \
  libc6-dev \
  libffi-dev \
  libglib2.0-dev \
  libpixman-1-dev \
  make \
  meson \
  ninja-build \
  pkgconf \
  python3 \
  python3-venv \
  sed \
  tar
```

These packages are intentionally close to QEMU's documented minimal Debian
build environment. QEMU has many optional dependencies, but this tutorial only
needs a system-mode `qemu-system-arm` binary for the Pico machine.

## Clone the QEMU Branch

The Pico 1 support used by this tutorial is not in stock QEMU yet. Clone the
experimental repository:

```sh
cd "$HOME/src"
git clone https://github.com/2xs/qemu-rp2040-pico.git
cd qemu-rp2040-pico
```

The repository currently uses `rp2040-pico` as its default branch. After a
fresh clone you should already be on that branch:

```sh
git branch --show-current
```

Expected output:

```text
rp2040-pico
```

If needed, you can check it out explicitly:

```sh
git checkout rp2040-pico
```

If the branch name changes while the work is being upstreamed, list the remote
branches and choose the branch that mentions RP2040/Pico support:

```sh
git branch -a
```

## Configure QEMU

Create a separate build directory and configure only the Arm system emulator:

```sh
mkdir -p build
cd build
../configure --target-list=arm-softmmu
```

Keeping only `arm-softmmu` makes the build faster and produces the binary used
throughout this tutorial:

```text
build/qemu-system-arm
```

## Build QEMU

Build the Arm system emulator:

```sh
ninja qemu-system-arm
```

On a multi-core machine, Ninja will parallelize the build automatically.

## Verify the Binary

From the build directory, check that the binary exists:

```sh
./qemu-system-arm --version
```

Then verify that the Pico machine is registered:

```sh
./qemu-system-arm -machine help | grep raspi-pico
```

Expected output should contain a line similar to:

```text
raspi-pico           Raspberry Pi Pico 1 / RP2040
```

The exact description text may evolve while the QEMU support is reviewed, but
the important part is the machine name:

```text
raspi-pico
```

## Smoke Test

Run QEMU with the Pico machine and no firmware:

```sh
./qemu-system-arm \
  -machine raspi-pico \
  -nographic
```

The Raspberry Pi Pico 1 has no display device. The `-nographic` option is only
used here to keep QEMU in the terminal and connect the default serial backend
to the console instead of opening any host UI window.

This command is not expected to print a useful firmware message yet. At this
stage, it only verifies that QEMU can instantiate the machine. Stop it with
`Ctrl-C`.

Congratulations! You are now able to emulate a Raspberry Pi Pico 1 with QEMU.

Next step: compile Pico 1 software with the Raspberry Pi Pico SDK. Go to
[`02-install-pico-sdk`](../02-install-pico-sdk/README.md).

## Troubleshooting

### `../configure` cannot find GLib

Install:

```sh
sudo apt install libglib2.0-dev pkgconf
```

### `../configure` cannot find Pixman

Install:

```sh
sudo apt install libpixman-1-dev
```

### `ninja` is not found

Install:

```sh
sudo apt install ninja-build
```

### `raspi-pico` does not appear in `-machine help`

Check that you are building the tutorial branch, not stock QEMU:

```sh
git branch --show-current
git log --oneline -5
```

You should be on a branch that contains RP2040/Pico support, such as:

```text
rp2040-pico
```

### QEMU starts but prints nothing

That is normal for this step. You have not loaded firmware yet. Step 3 will
build and run a UART hello-world firmware.
