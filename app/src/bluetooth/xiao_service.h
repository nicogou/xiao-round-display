#pragma once

/**
 * @file
 * @defgroup xiao_service XIAO GATT Service
 * @{
 * @brief XIAO GATT Service API.
 */

#include <zephyr/types.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/logging/log.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** @brief UUID of the XIAO Service. **/
#define BT_UUID_XIAO_SERVICE_VAL \
	BT_UUID_128_ENCODE(0xf4132900, 0xa7ec, 0x4710, 0x98ea, 0x0b790e8b575c)

/** @brief UUID of the Control Characteristic. **/
#define BT_UUID_CONTROL_VAL \
	BT_UUID_128_ENCODE(0xbc1c0001, 0xbffd, 0x4da5, 0xb41c, 0x2e3b89013d93)

#define BT_UUID_XIAO_SERVICE BT_UUID_DECLARE_128(BT_UUID_XIAO_SERVICE_VAL)
#define BT_UUID_CONTROL BT_UUID_DECLARE_128(BT_UUID_CONTROL_VAL)

	/** @brief XIAO service send status. */
	enum xiao_service_send_status
	{
		/** Send notification enabled. */
		XIAO_SERVICE_SEND_STATUS_ENABLED,
		/** Send notification disabled. */
		XIAO_SERVICE_SEND_STATUS_DISABLED,
	};

	/** @brief Pointers to the callback functions for service events. */
	struct xiao_service_cb
	{
		/* Control char */
		/** @brief Data received callback.
		 *
		 * The data has been received as a write request on the Control
		 * Characteristic.
		 *
		 * @param[in] conn  Pointer to connection object that has received data.
		 * @param[in] data  Received data.
		 * @param[in] len   Length of received data.
		 */
		void (*control_received)(struct bt_conn *conn,
								 const uint8_t *const data, uint16_t len);

		/** @brief Data sent callback.
		 *
		 * The data has been sent as a notification and written on the Control
		 * Characteristic.
		 *
		 * @param[in] conn Pointer to connection object, or NULL if sent to all
		 *                 connected peers.
		 */
		void (*control_sent)(struct bt_conn *conn);

		/** @brief Send state callback.
		 *
		 * Indicate the CCCD descriptor status of the Control characteristic.
		 *
		 * @param[in] status Send notification status.
		 */
		void (*control_send_enabled)(enum xiao_service_send_status status);

	};

	/**@brief Initialize the service.
	 *
	 * @details This function registers a GATT service with five characteristics,
	 *          Control.
	 *          A remote device that is connected to this service can send and receive data
	 *          on the Control Characteristic, and can receive session data ont the other 4.
	 *
	 *          @NGD: TBC-> When the remote enables notifications, it is notified when data is sent to the TX
	 *          Characteristic.
	 *
	 * @param[in] callbacks  Struct with function pointers to callbacks for service
	 *                       events. If no callbacks are needed, this parameter can
	 *                       be NULL.
	 *
	 * @retval 0 If initialization is successful.
	 *           Otherwise, a negative value is returned.
	 */
	int xiao_service_init(struct xiao_service_cb *callbacks);

	/**@brief Send data.
	 *
	 * @details This function sends data to a connected peer, or all connected
	 *          peers.
	 *
	 * @param[in] conn Pointer to connection object, or NULL to send to all
	 *                 connected peers.
	 * @param[in] data Pointer to a data buffer.
	 * @param[in] len  Length of the data in the buffer.
	 *
	 * @retval 0 If the data is sent.
	 *           Otherwise, a negative value is returned.
	 */
	int xiao_service_send(struct bt_conn *conn, const uint8_t *data, uint16_t len);

	/**@brief Get maximum data length that can be used for @ref xiao_service_send.
	 *
	 * @param[in] conn Pointer to connection Object.
	 *
	 * @return Maximum data length.
	 */
	static inline uint32_t xiao_service_get_mtu(struct bt_conn *conn)
	{
		/* According to 3.4.7.1 Handle Value Notification off the ATT protocol.
		 * Maximum supported notification is ATT_MTU - 3 */
		return bt_gatt_get_mtu(conn) - 3;
	}

	int control_send(struct bt_conn *conn, const uint8_t *data, uint16_t len);

#ifdef __cplusplus
}
#endif
