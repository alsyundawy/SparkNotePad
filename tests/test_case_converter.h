#pragma once
#include <QObject>
#include <QTest>
#include "ApplicationSettings.h"
#include "features/ThemeManager.h"
#include "features/ThemePaletteData.h"
#include "utils/CaseConverter.h"

class TestCaseConverter : public QObject {
    Q_OBJECT

private slots:
    // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
    void testConversions() {
        QCOMPARE(CaseConverter::toUpper("SparkNotePad"), QString("SPARKNOTEPAD"));
        QCOMPARE(CaseConverter::toLower("SparkNotePad"), QString("sparknotepad"));
        QCOMPARE(CaseConverter::toTitleCase("hello world"), QString("Hello World"));
        QCOMPARE(CaseConverter::toInvertCase("Hello World"), QString("hELLO wORLD"));
        QCOMPARE(CaseConverter::toSentenceCase("hello world. good morning."), QString("Hello world. Good morning."));
    }

    // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
    void testApplicationSettingsEncodingAndSaveAll() {
        ApplicationSettings settings;
        // Test defaults
        QCOMPARE(settings.autoDetectEncoding(), true);
        QCOMPARE(settings.confirmSaveAll(), false);

        // Test mutating and restoring
        settings.setAutoDetectEncoding(false);
        QCOMPARE(settings.autoDetectEncoding(), false);
        settings.setAutoDetectEncoding(true);
        QCOMPARE(settings.autoDetectEncoding(), true);

        settings.setConfirmSaveAll(true);
        QCOMPARE(settings.confirmSaveAll(), true);
        settings.setConfirmSaveAll(false);
        QCOMPARE(settings.confirmSaveAll(), false);
    }

    // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
    void testThemeManagerContrastAndSafety() {
        const double bwContrast = ThemeManager::contrastRatio(QColor(0, 0, 0), QColor(255, 255, 255));
        QVERIFY(bwContrast >= 20.0);

        // Test dark mode safety: black on dark card background (#1e1e2e) must be adjusted to readable
        const QColor darkCard("#1e1e2e");
        const QColor unsafeBlack("#000000");
        const QColor safeColor = ThemeManager::ensureContrast(unsafeBlack, darkCard, true, 3.5);
        QVERIFY(ThemeManager::contrastRatio(safeColor, darkCard) >= 3.5);
        QVERIFY(safeColor != unsafeBlack);

        // Test light mode safety: white on light card background (#fafbfc) must be adjusted to readable
        const QColor lightCard("#fafbfc");
        const QColor unsafeWhite("#ffffff");
        const QColor safeLightColor = ThemeManager::ensureContrast(unsafeWhite, lightCard, false, 3.5);
        QVERIFY(ThemeManager::contrastRatio(safeLightColor, lightCard) >= 3.5);
        QVERIFY(safeLightColor != unsafeWhite);

        // Verify across all theme palettes that txtPri contrast against card is WCAG AA (>= 4.5)
        for (const auto &palette : kAllThemePalettes) {
            const QColor card(palette.card);
            const QColor txtPri(palette.txtPri);
            const double ratio = ThemeManager::contrastRatio(txtPri, card);
            QVERIFY2(ratio >= 4.0,
                     qUtf8Printable(
                         QStringLiteral("Palette %1 contrast too low: %2").arg(QLatin1String(palette.id)).arg(ratio)));
        }
    }
};
