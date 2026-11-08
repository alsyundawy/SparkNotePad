/*
 *
 * This file is part of SparkNotepad.
 *
 * Copyright 2018-2026 Justin Dailey
 * Copyright 2025-2026 Harry Dertin Sutisna Alsyundawy <alsyundawy@gmail.com>
 * https://github.com/alsyundawy/SparkNotePad
 * ALSYUNDAWY IT SOLUTION <https://alsyundawy.com>
 *
 * SparkNotepad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Notepad Next is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Notepad Next.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <algorithm>
#include <concepts>
#include <iterator>
#include <vector>

#include <QEvent>
#include <QKeyEvent>

#include "BetterMultiSelection.h"

struct Selection {
    int caret;
    int anchor;

    Selection(int caret, int anchor) : caret(caret), anchor(anchor) {}

    int start() const { return qMin(caret, anchor); }
    int end() const { return qMax(caret, anchor); }
    int length() const { return end() - start(); }
    void set(int pos) {
        anchor = pos;
        caret = pos;
    }
    void offset(int offset) {
        anchor += offset;
        caret += offset;
    }
};

template <std::forward_iterator It>
It uniquify(It begin, It const end) {
    std::vector<It> v;
    v.reserve(static_cast<size_t>(std::distance(begin, end)));

    for (It i = begin; i != end; ++i) {
        v.push_back(i);
    }

    std::ranges::sort(v, [](const It &lhs, const It &rhs) {
        return (*lhs).start() < (*rhs).start() || (!((*rhs).start() < (*lhs).start()) && (*lhs).end() < (*rhs).end());
    });

    const auto subrange = std::ranges::unique(v, [](const It &lhs, const It &rhs) {
        return (*lhs).start() == (*rhs).start() && (*lhs).end() == (*rhs).end();
    });
    v.erase(subrange.begin(), subrange.end());

    std::ranges::sort(v);

    size_t j = 0;
    for (It i = begin; i != end; ++i) {
        if (j == v.size()) {
            break;
        }
        if (i == v[j]) {
            using std::iter_swap;
            iter_swap(i, begin);
            ++j;
            ++begin;
        }
    }
    return begin;
}

BetterMultiSelection::BetterMultiSelection(ScintillaNext *editor) : EditorDecorator(editor) {
    setObjectName("BetterMultiSelection");

    // Allow insertion of autocompletion at each cursor
    editor->autoCSetMulti(SC_MULTIAUTOC_EACH);

    editor->installEventFilter(this);
}

bool BetterMultiSelection::handleControlNavigation(int key, bool isShiftPressed) {
    switch (key) {
        case Qt::Key_Left:
            EditSelections(SimpleEdit(isShiftPressed ? SCI_WORDLEFTEXTEND : SCI_WORDLEFT));
            return true;
        case Qt::Key_Right:
            EditSelections(SimpleEdit(isShiftPressed ? SCI_WORDRIGHTENDEXTEND : SCI_WORDRIGHT));
            return true;
        case Qt::Key_Back:
            EditSelections(SimpleEdit(SCI_DELWORDLEFT));
            return true;
        case Qt::Key_Delete:
            EditSelections(SimpleEdit(SCI_DELWORDRIGHT));
            return true;
        default:
            return false;
    }
}

bool BetterMultiSelection::handleStandardNavigation(int key, bool isShiftPressed) {
    switch (key) {
        case Qt::Key_Escape: {
            const auto caret = static_cast<int>(editor->selectionNCaret(editor->mainSelection()));
            editor->setSelection(caret, caret);
            return true;
        }
        case Qt::Key_Left:
            EditSelections(SimpleEdit(isShiftPressed ? SCI_CHARLEFTEXTEND : SCI_CHARLEFT));
            return true;
        case Qt::Key_Right:
            EditSelections(SimpleEdit(isShiftPressed ? SCI_CHARRIGHTEXTEND : SCI_CHARRIGHT));
            return true;
        case Qt::Key_Home:
            EditSelections(SimpleEdit(isShiftPressed ? SCI_VCHOMEWRAPEXTEND : SCI_VCHOMEWRAP));
            return true;
        case Qt::Key_End:
            EditSelections(SimpleEdit(isShiftPressed ? SCI_LINEENDWRAPEXTEND : SCI_LINEENDWRAP));
            return true;
        case Qt::Key_Return:
            if (!editor->autoCActive()) {
                EditSelections(SimpleEdit(SCI_NEWLINE));
                return true;
            }
            break;
        case Qt::Key_Up:
            if (!editor->autoCActive()) {
                EditSelections(SimpleEdit(isShiftPressed ? SCI_LINEUPEXTEND : SCI_LINEUP));
                return true;
            }
            break;
        case Qt::Key_Down:
            if (!editor->autoCActive()) {
                EditSelections(SimpleEdit(isShiftPressed ? SCI_LINEDOWNEXTEND : SCI_LINEDOWN));
                return true;
            }
            break;
        default:
            break;
    }
    return false;
}

bool BetterMultiSelection::eventFilter(QObject *obj, QEvent *event) {
    Q_UNUSED(obj);

    if (event->type() != QEvent::KeyPress) {
        return false;
    }

    const auto *keyEvent = static_cast<const QKeyEvent *>(event);
    if ((keyEvent->modifiers() & Qt::AltModifier) || editor->selections() <= 1) {
        return false;
    }

    const bool isShiftPressed = (keyEvent->modifiers() & Qt::ShiftModifier);
    if (keyEvent->modifiers() & Qt::ControlModifier) {
        return handleControlNavigation(keyEvent->key(), isShiftPressed);
    }
    return handleStandardNavigation(keyEvent->key(), isShiftPressed);
}

void BetterMultiSelection::notify(const Scintilla::NotificationData * /*pscn*/) {
    // Intentionally empty: notification handling not required for multi-selection
}

QVector<Selection> BetterMultiSelection::GetSelections() const {
    QVector<Selection> selections;

    const auto num = static_cast<int>(editor->selections());
    selections.reserve(num);
    for (int i = 0; i < num; ++i) {
        const auto caret = static_cast<int>(editor->selectionNCaret(i));
        const auto anchor = static_cast<int>(editor->selectionNAnchor(i));
        selections.append(Selection{caret, anchor});
    }

    return selections;
}

void BetterMultiSelection::SetSelections(const QVector<Selection> &selections) {
    for (auto i = 0; i < selections.size(); ++i) {
        if (i == 0) {
            editor->setSelection(selections[i].caret, selections[i].anchor);
        } else {
            editor->addSelection(selections[i].caret, selections[i].anchor);
        }
    }
}

template <typename EditFn>
void BetterMultiSelection::EditSelections(EditFn &&edit) {
    auto selections = GetSelections();

    editor->clearSelections();

    std::ranges::sort(selections, [](const Selection &lhs, const Selection &rhs) {
        return lhs.start() < rhs.start() || (!(rhs.start() < lhs.start()) && lhs.end() < rhs.end());
    });

    editor->beginUndoAction();

    int totalOffset = 0;
    for (auto &selection : selections) {
        selection.offset(totalOffset);
        const auto length = static_cast<int>(editor->length());

        edit(selection);

        totalOffset += static_cast<int>(editor->length()) - length;
    }

    editor->endUndoAction();

    selections.erase(uniquify(selections.begin(), selections.end()), selections.end());

    SetSelections(selections);
}

std::function<void(Selection &selection)> BetterMultiSelection::SimpleEdit(int message) {
    return [this, message](Selection &selection) {
        editor->setSelection(selection.caret, selection.anchor);
        editor->send(message);

        selection.caret = static_cast<int>(editor->selectionNCaret(0));
        selection.anchor = static_cast<int>(editor->selectionNAnchor(0));
    };
}
