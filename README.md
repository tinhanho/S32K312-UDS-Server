# MWCT2016S-UDS-Server

UDS application, CAN bootloader, and A/B swap examples for the NXP S32K3 platform.

> Reference: [NXP Unified Bootloader Demo](https://community.nxp.com/t5/S32K-Knowledge-Base/Unified-bootloader-Demo/ta-p/1423099).

## Requirements

- S32K312 SDK and configuration tools with C40_Ip and Power_Ip support.
- HSE firmware when relocating the A/B swap active and passive blocks.
- ECUBus for UDS test execution.
- PCAN tools and a compatible CAN interface.
- The [PCAN Tutorial](PCAN%20Tutorial.pdf) for CAN hardware setup.

## Repository Layout

| Directory | Purpose |
| --- | --- |
| `can_bootloader/` | CAN bootloader, Flash programming, boot control, HAL, and UDS stack. |
| `AB Swap/` | Standalone application example for A/B application deployment. |
| `ECUBus Config/` | ECUBus bootloader and A/B swap test configurations. |

## Architecture

The project is organized into three usage scenarios:

1. **Application.** Uses the UDS and transport stacks to communicate over CAN or LIN.
2. **CAN bootloader.** Validates the installed application, handles UDS download services, programs Flash, and jumps to the application.
3. **A/B swap.** Provides a separate application-side example with Flash, CRC, UDS algorithm, watchdog, and transport modules.

The bootloader stores application metadata, including the application start address and fingerprint, in a dedicated status area. The download flow uses CRC validation before accepting programmed data. The current implementation uses a 16-bit CRC value.

## Application Integration

Import `application/FIFO` and `application/UDS_stack` into the application project.

Initialize the transport and UDS layers from the application entry point:

```c
UDS_Init();
TP_Init();
```

Call the periodic functions from the 1 ms task:

```c
TP_SystemTickCtl();
UDS_SystemTickCtl();
```

Call the transport and UDS main functions from the main loop. The FlexCAN callback should forward received frames to `TP_DriverWriteDataInTP` and notify the transport layer after transmission with `TP_DoTxMsgSuccesfulCallback`.

For outgoing data, read frames with `TP_DriverReadDataFromTP` and send them through the configured FlexCAN mailbox. The application configuration supports standard CAN and can be changed to CAN FD through the transport configuration macros.

## CAN Bootloader

Use the `Boot_Test` project configuration instead of `S32K312_user_config` when running the documented bootloader test flow.

1. Import the modules under `can_bootloader/` into the bootloader project.
2. Add the C40_Ip and Power_Ip modules in the configuration tool.
3. Configure CAN mailboxes, transport IDs, and the application Flash range for the target.
4. Ensure the bootloader performs the required Flash initialization before UDS download operations.
5. Program the target with `bin/S32K312_CAN_bootloader_RTD2d0.bin` when using the checked-in image.
6. Open `ECUBus Config/Boot_Test/config.json` in ECUBus to execute the bootloader test sequence.

The UDS flow includes diagnostic session control, security access, download, data transfer, transfer exit, routine control, fingerprint writing, and ECU reset. Flash programming stores the received fingerprint and validates the downloaded application before the bootloader jumps to it.

## A/B Swap Demo

The `AB Swap/` directory is a standalone application example. Its entry points are:

- `AB Swap/main.c`.
- `AB Swap/APP_Demo/APP_Demo.c`.
- `AB Swap/fls_app/` for Flash application handling.
- `AB Swap/UDS_stack/` for CAN/LIN transport and UDS services.

The demo initializes the BSP, UDS, transport, and Flash application modules. It also provides CAN receive/transmit callbacks and a `SendMsgMainFun` implementation for forwarding transport frames through FlexCAN.

The A/B swap flow can use HSE firmware to relocate the active block and passive block. This allows the application block assignments to be updated while preserving the active/passive deployment model.

## Test Assets

- Bootloader image: `bin/S32K312_CAN_bootloader_RTD2d0.bin`.
- ECUBus test flow: `ECUBus Config/Boot_Test/config.json`.
- Hardware setup notes: `PCAN Tutorial.pdf`.

<!-- ## Memory Allocation -->