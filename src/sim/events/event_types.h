#pragma once

enum class EventType {
    /* API events */
    REQUEST_ARRIVAL,
    REQUEST_PROCESSING,

    /* Database events */
    DB_REQUEST_ARRIVAL,
    DB_REQUEST_SEND,

    /* Network events */
    NETWORK_LINK_ARRIVAL,

    /* Other */
    TIMER,
    FAILURE,
    UNKNOWN
};
