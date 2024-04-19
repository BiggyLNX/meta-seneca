inherit logging
FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI += "file://usb-gadget.cfg"

KERNEL_FEATURES:append = "usb-gadget.cfg"

do_patch:prepend() {
    bb.warn("Running do_patch()")
}
