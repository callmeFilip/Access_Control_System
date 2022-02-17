#ifndef PN532_H
#define PN532_H

#include <nfc/nfc.h>

#include "IOManager.hpp"

class PN532
{
private:
    nfc_device *pn532_dev;
    nfc_context *context;
    IOManager *const m_IO_manager;

    int log(const std::string &msg) const;

public:
    PN532();
    PN532(nfc_connstring &dev_name);
    ~PN532();

    int poll(nfc_modulation *modulation, nfc_target *target);
};

#endif // PN532