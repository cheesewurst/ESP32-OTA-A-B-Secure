                OTA UPDATE INSTALLED
                       │
                       ▼
               Firmware state = NEW
                       │
                       ▼
                 Device Reboots
                       │
                       ▼
                Bootloader starts
                       │
                       ▼
        Is state = NEW ? ──────── No ─────────► Boot normally
                       │
                       Yes
                       │
                       ▼
       Bootloader changes state → PENDING_VERIFY
                       │
                       ▼
                Boot new firmware
                       │
                       ▼
           Application starts executing
                       │
                       ▼
         Check firmware state with:
         esp_ota_get_state_partition()
                       │
                       ▼
        Is state = PENDING_VERIFY ?
                       │
                       ▼
              Run diagnostics
                       │
          ┌────────────┴────────────┐
          │                         │
          ▼                         ▼
Diagnostics OK              Diagnostics FAILED
          │                         │
          ▼                         ▼
esp_ota_mark_app_           esp_ota_mark_app_
valid_cancel_rollback()     invalid_rollback_and_reboot()
          │                         │
          ▼                         ▼
   State → VALID              State → INVALID
          │                         │
          ▼                         ▼
  Firmware becomes             Immediate reboot
    permanent                        │
                                     ▼
                             Bootloader starts
                                     │
                                     ▼
                         INVALID firmware skipped
                                     │
                                     ▼
                        Previous firmware boots