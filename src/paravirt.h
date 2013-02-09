#ifndef __PV_H
#define __PV_H

#include "config.h" // CONFIG_*
#include "biosvar.h" // GET_GLOBAL

// Types of paravirtualized platforms.
#define PF_QEMU     (1<<0)
#define PF_XEN      (1<<1)
#define PF_KVM      (1<<2)

// misc.c
extern int PlatformRunningOn;

static inline int runningOnQEMU(void) {
    return CONFIG_QEMU || (
        CONFIG_QEMU_HARDWARE && GET_GLOBAL(PlatformRunningOn) & PF_QEMU);
}
static inline int runningOnXen(void) {
    return CONFIG_XEN && GET_GLOBAL(PlatformRunningOn) & PF_XEN;
}
static inline int runningOnKVM(void) {
    return CONFIG_QEMU && GET_GLOBAL(PlatformRunningOn) & PF_KVM;
}

// QEMU "firmware config (fw_cfg)" interface
#define QEMU_CFG_SIGNATURE              0x00
#define QEMU_CFG_ID                     0x01
#define QEMU_CFG_UUID                   0x02
#define QEMU_CFG_NUMA                   0x0d
#define QEMU_CFG_BOOT_MENU              0x0e
#define QEMU_CFG_MAX_CPUS               0x0f
#define QEMU_CFG_FILE_DIR               0x19
#define QEMU_CFG_ARCH_LOCAL             0x8000
#define QEMU_CFG_ACPI_TABLES            (QEMU_CFG_ARCH_LOCAL + 0)
#define QEMU_CFG_SMBIOS_ENTRIES         (QEMU_CFG_ARCH_LOCAL + 1)
#define QEMU_CFG_IRQ0_OVERRIDE          (QEMU_CFG_ARCH_LOCAL + 2)
#define QEMU_CFG_E820_TABLE             (QEMU_CFG_ARCH_LOCAL + 3)

extern int qemu_cfg_present;

void qemu_ramsize_preinit(void);
void qemu_biostable_setup(void);
void qemu_cfg_preinit(void);
int qemu_cfg_show_boot_menu(void);
void qemu_cfg_get_uuid(u8 *uuid);
int qemu_cfg_irq0_override(void);
u16 qemu_cfg_acpi_additional_tables(void);
u16 qemu_cfg_next_acpi_table_len(void);
void *qemu_cfg_next_acpi_table_load(void *addr, u16 len);
u16 qemu_cfg_smbios_entries(void);
size_t qemu_cfg_smbios_load_field(int type, size_t offset, void *addr);
int qemu_cfg_smbios_load_external(int type, char **p, unsigned *nr_structs,
                                  unsigned *max_struct_size, char *end);
int qemu_cfg_get_numa_nodes(void);
void qemu_cfg_get_numa_data(u64 *data, int n);
u16 qemu_cfg_get_max_cpus(void);
struct e820_reservation {
    u64 address;
    u64 length;
    u32 type;
};
u32 qemu_cfg_e820_entries(void);
void* qemu_cfg_e820_load_next(void *addr);
void qemu_romfile_init(void);

#endif
