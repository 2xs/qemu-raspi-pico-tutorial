# Tutorial TODO

This file tracks possible future tutorial steps that are useful but not part
of the first SDK-oriented path.

## Possible Future Steps

- [ ] Document `strict-uart-pins=off`.

  This is mostly a hardware-fidelity switch. SDK-based examples normally
  configure the UART pins correctly, so it is not a priority for the main
  tutorial flow. It may become useful in a troubleshooting or low-level
  pinmux step.

- [ ] Add a short GDB debugging step.

  QEMU's `-S -gdb tcp::1234` workflow is standard QEMU usage rather than
  Pico-specific behaviour. A future step could still show how to attach
  `arm-none-eabi-gdb` to a Pico firmware image.
