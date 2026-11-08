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

#include <QTest>
#include <QObject>
#include "docks/ClipboardHistoryDock.h"
#include "docks/CharacterInfoDock.h"

class TestClipboardHistoryDock : public QObject {
    Q_OBJECT

private slots:
    void testFifoQueueAndCapacity() const {
        ClipboardHistoryDock dock;
        dock.setMaxEntries(20);

        for (int i = 0; i < 25; ++i) {
            dock.addClip(QStringLiteral("Item %1").arg(i));
        }

        const QStringList entries = dock.entries();
        QCOMPARE(entries.size(), 20);
        // The most recent item should be at the top or latest in queue
        QCOMPARE(entries.first(), QStringLiteral("Item 24"));
        QCOMPARE(entries.last(), QStringLiteral("Item 5"));
    }

    void testDeduplicationConsecutive() const {
        ClipboardHistoryDock dock;
        dock.addClip(QStringLiteral("DuplicateMe"));
        dock.addClip(QStringLiteral("DuplicateMe"));
        dock.addClip(QStringLiteral("Different"));
        dock.addClip(QStringLiteral("DuplicateMe"));

        const QStringList entries = dock.entries();
        // Consecutive duplicates are ignored; non-consecutive is allowed
        QCOMPARE(entries.size(), 3);
        QCOMPARE(entries.at(0), QStringLiteral("DuplicateMe"));
        QCOMPARE(entries.at(1), QStringLiteral("Different"));
        QCOMPARE(entries.at(2), QStringLiteral("DuplicateMe"));
    }

    void testClearHistory() const {
        ClipboardHistoryDock dock;
        dock.addClip(QStringLiteral("Alpha"));
        dock.addClip(QStringLiteral("Beta"));
        QCOMPARE(dock.entries().size(), 2);

        dock.clear();
        QCOMPARE(dock.entries().size(), 0);
    }

    void testIgnoreEmptyClips() const {
        ClipboardHistoryDock dock;
        dock.addClip(QString());
        dock.addClip(QStringLiteral("   "));
        QCOMPARE(dock.entries().size(), 0);
    }

    void testCharacterInfoDockCodePoints() const {
        CharacterInfoDock dock;
        dock.updateCharacter(0x0041);  // 'A'
        QCOMPARE(dock.currentCodePoint(), static_cast<uint32_t>(0x0041));
        QCOMPARE(dock.characterName(), QStringLiteral("LATIN CAPITAL LETTER A"));

        dock.updateCharacter(0x20AC);  // '€'
        QCOMPARE(dock.currentCodePoint(), static_cast<uint32_t>(0x20AC));
        QCOMPARE(dock.characterName(), QStringLiteral("Currency Symbols U+20AC"));
    }
};
