DESCRIPTION = "Recipe for I2C User space integreation for TSC2007 (Touchscreen Controller Chip)."
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "git://https://github.com/BiggyLNX/meta-seneca.git;branch=master;protocol=https"
SRCREV = "17464fa2771c460fdc82c2c87d60ad6c5bb814ba"

S = "${WORKDIR}/git"

do_compile() {
    ${CC} paint.c -o paint-app ${LDFLAGS}
}

do_install() {
    install -d ${D}${bindir}
    install -m 0755 paint-app ${D}${bindir}
}

FILES_${PN} += "${bindir}/paint-app"
