#include "mainwindow.h"
#include <QApplication>

#include <libvmi/libvmi.h>

bool connect_vmi();


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    if (true == connect_vmi()) {
        w.update_log("Initialized LibVMI library.\n");
    }
    else {
        w.update_log("Failed to init LibVMI library.\n");
    }

    return a.exec();
}


bool connect_vmi() {
    // assuming initialization code can go here..
    vmi_instance_t vmi;
    addr_t list_head = 0, cur_list_entry = 0, next_list_entry = 0;
    addr_t current_process = 0;
    char *procname = NULL;
    vmi_pid_t pid = 0;
    unsigned long tasks_offset = 0, pid_offset = 0, name_offset = 0;
    status_t status;

    char *name = "win10";

    /* initialize the libvmi library */
    if (VMI_FAILURE ==
            vmi_init_complete(&vmi, name, VMI_INIT_DOMAINNAME, NULL,
                              VMI_CONFIG_GLOBAL_FILE_ENTRY, NULL, NULL)) {

        return false;
    }
    return true;
}
