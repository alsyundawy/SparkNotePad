#pragma once
/*
 * This file is part of SparkNotePad.
 *
 * Copyright 2026 Harry Dertin Sutisna Alsyundawy <alsyundawy@gmail.com>
 * https://github.com/alsyundawy/SparkNotePad
 * ALSYUNDAWY IT SOLUTION <https://alsyundawy.com>
 *
 * SparkNotePad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include <QObject>
#include <QSslConfiguration>
#include <QNetworkRequest>

class NetworkSecurityPolicy {
public:
    NetworkSecurityPolicy() = default;

    /**
     * @brief Configures global SSL/TLS security baseline.
     * Enforces TLS 1.2/1.3 minimums, rejects insecure ciphers, and restricts
     * CA certificate lookups from CWD (mitigating CVE-2025-14575).
     */
    static void initializeGlobalSecurityPolicy();

    /**
     * @brief Applies secure TLS configuration to an outbound QNetworkRequest.
     */
    static void applySecureTlsConfiguration(QNetworkRequest &request);

    /**
     * @brief Checks if strict offline air-gap mode is active.
     * When true, all outbound internet requests must be dropped.
     */
    static bool isOfflineModeEnabled();

    /**
     * @brief Sets strict offline air-gap mode.
     */
    static void setOfflineModeEnabled(bool enabled);

private:
    static bool s_offlineMode;
};
