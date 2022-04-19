

#pragma once

// Reboots the system into the bootloader, making sure
// it enters in DFU mode.
void reboot_into_bootloader() ;

// Clears reboot information so we reboot in "normal" mode
void clear_reboot_flags() ;

// Returns whether we were rebooted into DFU mode
int rebooted_into_dfu() ;



