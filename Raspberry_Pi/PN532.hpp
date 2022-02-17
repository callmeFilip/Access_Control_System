#ifndef PN532_H
#define PN532_H

#include <nfc/nfc.h>

#include "IOManager.hpp"

class PN532
{
private:
    nfc_device *m_pn532_dev;
    nfc_context *m_context;
    IOManager &m_IO_manager;

    int log(const std::string &msg) const;
    int init(const nfc_connstring *dev_name);

public:
    PN532(IOManager &IO_manager, nfc_connstring *dev_name);
    ~PN532();

    int poll(const nfc_modulation &modulation, nfc_target &target);
};

#endif // PN532