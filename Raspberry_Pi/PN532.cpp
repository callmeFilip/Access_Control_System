#include "PN532.hpp"
#include "IOManager.hpp"

/**
 * @brief Construct a new PN532::PN532 object
 * Initialize nfc
 * @param IO_manager IO_manager instance responsible for logging
 * @param dev_name Device name. If null default NFC device(/etc/nfc/libnfc.conf) will be chosen.
 */
PN532::PN532(IOManager &IO_manager, nfc_connstring *dev_name)
    : m_pn532_dev(NULL), m_context(NULL), m_IO_manager(IO_manager)
{
    init(dev_name);
}

/**
 * @brief Destroy the PN532::PN532 object
 * Close connection to device and exit libnfc
 */
PN532::~PN532()
{
    nfc_close(m_pn532_dev);
    nfc_exit(m_context);
#ifdef DEBUG
    log("PN532 closed successfully");
#endif
}

/**
 * @brief Initialize the library and open nfc device
 *
 * @param dev_name Device name. If null default NFC device(/etc/nfc/libnfc.conf) will be chosen.
 * @return int
 */
int PN532::init(const nfc_connstring *dev_name)
{
    nfc_init(&m_context);

    // Initialize context
    if (m_context == NULL)
    {
#ifdef DEBUG
        log("Unable to initialize libnfc (malloc)");
#endif
        return 1;
    }

    // Open nfc device
    m_pn532_dev = nfc_open(m_context, *dev_name);
    if (nfc_initiator_init(m_pn532_dev) < 0)
    {
#ifdef DEBUG
        log("PN532 could not open device");
#endif
        return 1;
    }

    log("PN532 initialized successfully");
    return 0;
}

/**
 * @brief Poll for tag
 *
 * @param modulation Tag type
 * @param target Result from scan
 * @return int
 */
int PN532::poll(const nfc_modulation &modulation, nfc_target &target)
{
    // Start polling
    if (nfc_initiator_select_passive_target(m_pn532_dev, modulation, NULL, 0, &target) > 0)
    {
#ifdef DEBUG
        log("PN532 read successfully");
#endif
        return 0;
    }

#ifdef DEBUG
    log("PN532 did not read successfully");
#endif

    return 1;
}

/**
 * @brief Logging data to log.txt
 *
 * @param msg Message to log
 * @return int
 */
int PN532::log(const std::string &msg) const
{
    return m_IO_manager.write(msg);
}
