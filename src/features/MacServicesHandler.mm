/*
 * This file is part of SparkNotePad.
 * Copyright 2026 Harry DS Alsyundawy (SparkNotePad Maintainer)
 *
 * SparkNotePad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "MacServicesHandler.h"
#include "SparkNotePadApplication.h"
#include "dialogs/MainWindow.h"
#include "ScintillaNext.h"
#include <QWidget>

#import <Cocoa/Cocoa.h>

@interface SparkNotePadServicesProvider : NSObject
{
    MacServicesHandler *m_handler;
}
- (instancetype)initWithHandler:(MacServicesHandler *)handler;
- (void)openSelectionService:(NSPasteboard *)pboard userData:(NSString *)userData error:(NSString **)error;
- (void)appendSelectionService:(NSPasteboard *)pboard userData:(NSString *)userData error:(NSString **)error;
- (void)openFileService:(NSPasteboard *)pboard userData:(NSString *)userData error:(NSString **)error;
@end

@implementation SparkNotePadServicesProvider

- (instancetype)initWithHandler:(MacServicesHandler *)handler {
    if (self = [super init]) {
        m_handler = handler;
    }
    return self;
}

- (void)openSelectionService:(NSPasteboard *)pboard userData:(NSString *)userData error:(NSString **)error {
    (void)userData;
    (void)error;
    if (![pboard canReadItemWithDataConformingToTypes:@[NSPasteboardTypeString]]) {
        return;
    }
    NSString *str = [pboard stringForType:NSPasteboardTypeString];
    if (str && m_handler) {
        QString qstr = QString::fromUtf8([str UTF8String]);
        dispatch_async(dispatch_get_main_queue(), ^{
            m_handler->handleOpenSelection(qstr);
        });
    }
}

- (void)appendSelectionService:(NSPasteboard *)pboard userData:(NSString *)userData error:(NSString **)error {
    (void)userData;
    (void)error;
    if (![pboard canReadItemWithDataConformingToTypes:@[NSPasteboardTypeString]]) {
        return;
    }
    NSString *str = [pboard stringForType:NSPasteboardTypeString];
    if (str && m_handler) {
        QString qstr = QString::fromUtf8([str UTF8String]);
        dispatch_async(dispatch_get_main_queue(), ^{
            m_handler->handleAppendSelection(qstr);
        });
    }
}

- (void)openFileService:(NSPasteboard *)pboard userData:(NSString *)userData error:(NSString **)error {
    (void)userData;
    (void)error;
    NSArray *classes = @[[NSURL class]];
    NSDictionary *options = @{NSPasteboardURLReadingFileURLsOnlyKey: @YES};
    NSArray *urls = [pboard readObjectsForClasses:classes options:options];
    if (urls && urls.count > 0 && m_handler) {
        QStringList paths;
        for (NSURL *url in urls) {
            paths << QString::fromUtf8([[url path] UTF8String]);
        }
        dispatch_async(dispatch_get_main_queue(), ^{
            m_handler->handleOpenFiles(paths);
        });
    }
}

@end

static SparkNotePadServicesProvider *s_servicesProvider = nil;

MacServicesHandler *MacServicesHandler::instance()
{
    static MacServicesHandler s_instance;
    return &s_instance;
}

MacServicesHandler::MacServicesHandler(QObject *parent)
    : QObject(parent)
{
}

MacServicesHandler::~MacServicesHandler()
{
    if (s_servicesProvider) {
        [s_servicesProvider release];
        s_servicesProvider = nil;
    }
}

void MacServicesHandler::initialize(SparkNotePadApplication *app)
{
    m_app = app;
    @autoreleasepool {
        if (!s_servicesProvider) {
            s_servicesProvider = [[SparkNotePadServicesProvider alloc] initWithHandler:this];
            [NSApp setServicesProvider:s_servicesProvider];
            NSUpdateDynamicServices();
        }
    }
}

void MacServicesHandler::setActiveWindow(MainWindow *window)
{
    m_activeWindow = window;
}

void MacServicesHandler::showEmojiAndSymbolsPalette()
{
    @autoreleasepool {
        [NSApp orderFrontCharacterPalette:nil];
    }
}

void MacServicesHandler::postVoiceOverAnnouncement(const QString &text)
{
    @autoreleasepool {
        NSString *nsMsg = [NSString stringWithUTF8String:text.toUtf8().constData()];
        NSDictionary *info = @{
            NSAccessibilityAnnouncementKey: nsMsg,
            NSAccessibilityPriorityKey: @(NSAccessibilityPriorityHigh)
        };
        NSAccessibilityPostNotificationWithUserInfo(
            NSApp,
            NSAccessibilityAnnouncementRequestedNotification,
            info
        );
    }
}

void MacServicesHandler::activateApplication()
{
    @autoreleasepool {
        [NSApp activateIgnoringOtherApps:YES];
    }
}

void MacServicesHandler::makeWidgetWindowTranslucent(const QWidget *w)
{
    if (!w) {
        return;
    }
    @autoreleasepool {
        const WId viewId = w->winId();
        if (!viewId) {
            return;
        }
        NSView *view = (__bridge NSView *)reinterpret_cast<void *>(viewId);
        if (!view) {
            return;
        }
        NSWindow *window = [view window];
        if (!window) {
            return;
        }
        [window setOpaque:NO];
        [window setBackgroundColor:[NSColor clearColor]];
        [window invalidateShadow];
    }
}


void MacServicesHandler::handleOpenSelection(const QString &text)
{
    emit openSelectionRequested(text);
    if (m_activeWindow) {
        m_activeWindow->newFile();
        ScintillaNext *ed = m_activeWindow->currentEditor();
        if (ed) {
            ed->setText(text.toUtf8().constData());
            ed->grabFocus();
        }
        m_activeWindow->raise();
        m_activeWindow->activateWindow();
    }
}

void MacServicesHandler::handleAppendSelection(const QString &text)
{
    emit appendSelectionRequested(text);
    if (m_activeWindow) {
        ScintillaNext *ed = m_activeWindow->currentEditor();
        if (!ed) {
            handleOpenSelection(text);
            return;
        }
        QByteArray bytes = text.toUtf8();
        ed->appendText(bytes.length(), bytes.constData());
        ed->grabFocus();
        m_activeWindow->raise();
        m_activeWindow->activateWindow();
    }
}

void MacServicesHandler::handleOpenFiles(const QStringList &filePaths)
{
    emit openFilesRequested(filePaths);
    if (m_activeWindow) {
        for (const QString &fp : filePaths) {
            m_activeWindow->openFile(fp);
        }
        m_activeWindow->raise();
        m_activeWindow->activateWindow();
    }
}
