#include <com/sun/star/lang/XMultiServiceFactory.hpp>

#include <com/sun/star/frame/XComponentLoader.hpp>

#include <com/sun/star/text/XTextDocument.hpp>

#include <com/sun/star/text/XText.hpp>

#include <com/sun/star/text/XTextTable.hpp>

#include <com/sun/star/text/XTextContent.hpp>

#include <com/sun/star/table/XCell.hpp>

#include <com/sun/star/table/XTable.hpp>

#include "my_addition.h"

#include <algorithm>

#include <cstdlib>

#include <ctime>

#include <cctype>

#include <string>

#include <uchar.h>

#include <iostream>

#include <map>


using namespace com::sun::star::uno;
using namespace com::sun::star::lang;
using namespace com::sun::star::beans;
using namespace com::sun::star::frame;
using namespace com::sun::star::text;
using namespace com::sun::star::table;
using namespace com::sun::star::util;
using::rtl::OUString;

std::u16string
const cyrillic = u"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдеёжзийклмнопрстуфхцчшщъыьэюя";
std::u16string
const latin = u"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
std::u16string
const mix = u"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyzАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдеёжзийклмнопрстуфхцчшщъыьэюя";

bool MyIsAlpha(char16_t letter) {
    return std::isalpha((unsigned char) letter) || cyrillic.find(letter) != std::string::npos;
}

void CreateNewDocument(Reference < XFrame > & rxFrame, int keyboard_layout, int globalWordCnt, int globalWordLenght) {
    if (not rxFrame.is())
        return;

    Reference < XComponentLoader > rComponentLoader(rxFrame, UNO_QUERY);
    Reference <XComponent> xWriterComponent = rComponentLoader -> loadComponentFromURL(
        OUString::createFromAscii("private:factory/swriter"),
        OUString::createFromAscii("_blank"),
        0,
        Sequence < ::com::sun::star::beans::PropertyValue > ());
    Reference < XTextDocument > xTextDocument(xWriterComponent, UNO_QUERY);
    Reference < XText > xText = xTextDocument -> getText();
    Reference < XTextCursor > xTextCursor = xText -> createTextCursor();
    OUString allword = "";
    srand(time(NULL));
    for (std::size_t i = 0; i < globalWordCnt; i++) {
        OUString word = "";
        if (!keyboard_layout) {
            for (std::size_t j = 0; j < globalWordLenght; j++) {
                word += (OUString) latin[rand() % 52];
            }
        } else if (keyboard_layout == 1) {
            for (std::size_t j = 0; j < globalWordLenght; j++) {
                word += (OUString) cyrillic[rand() % 66];
            }
        } else if (keyboard_layout == 2) {
            for (std::size_t j = 0; j < globalWordLenght; j++) {
                word += (OUString) mix[rand() % 118];
            }
        }
        allword += word + " ";

    }
    xText -> insertString(xTextCursor, allword, false);
    return;
}

void MakeStatistics(Reference < XFrame > & mxFrame) {
    std::map < int, int > myStat;
    Reference < XTextDocument > xTextDocument(mxFrame -> getController() -> getModel(), UNO_QUERY);
    Reference < XText > xText = xTextDocument -> getText();
    Reference < XTextCursor > xTextCursor = xText -> createTextCursor();
    std::size_t curSum = 0, curSize, textSize;
    textSize = (xText -> getString()).getLength();
    Reference < XPropertySet > xCursorProps(xTextCursor, UNO_QUERY);
    while (curSum < textSize && xTextCursor -> goRight(1, true)) {
        curSize = 0;
        while (curSize < (textSize - curSum) && MyIsAlpha((xTextCursor -> getString())[curSize])) {
            xTextCursor -> goRight(1, true);
            curSize++;
        }
        if (curSize != (textSize - curSum)) {
            xTextCursor -> goLeft(1, true);

        }

        if (curSize != 0) {
            //std::cout <<xTextCursor -> getString() << " " << curSize << "\n";        
            if (!(myStat.insert(std::pair < int, int > (curSize, 1))).second) {
                myStat[curSize]++;
            }
        }

        if (xTextCursor -> goRight(1, true)) {
            xTextCursor -> collapseToEnd();
            curSum += curSize + 1;
        } else {
            break;
        }
    }

    xTextCursor -> gotoEnd(false);

    Reference < XMultiServiceFactory > oDocMSF(xTextDocument, UNO_QUERY);
    Reference < XTextTable > xTable(oDocMSF -> createInstance(
        OUString::createFromAscii("com.sun.star.text.TextTable")), UNO_QUERY);
    if (!xTable.is()) {
        printf("Erreur creation XTextTable interface !\n");
    }

    xTable -> initialize(myStat.size() + 1, 2);
    Reference < XTextRange > xTextRange = xText -> getEnd();
    Reference < XTextContent > xTextContent(xTable, UNO_QUERY);
    xText -> insertTextContent(xTextRange, xTextContent, (unsigned char) 0);

    Reference < XCell > xCell = xTable -> getCellByName(OUString::createFromAscii("A1"));
    xText = Reference < XText > (xCell, UNO_QUERY);
    xTextCursor = xText -> createTextCursor();
    xTextCursor -> setString(OUString::createFromAscii("Word Length"));

    xCell = xTable -> getCellByName(OUString::createFromAscii("B1"));
    xText = Reference < XText > (xCell, UNO_QUERY);
    xTextCursor = xText -> createTextCursor();
    xTextCursor -> setString(OUString::createFromAscii("Word Count"));

    int i = 2;
    for (std::map < int, int > ::iterator it = myStat.begin(); it != myStat.end(); ++it) {
        xCell = xTable -> getCellByName(OUString::createFromAscii(((char)('A') + std::to_string(i)).c_str()));
        xText = Reference < XText > (xCell, UNO_QUERY);
        xTextCursor = xText -> createTextCursor();
        xCell -> setValue(it -> first);
        xText = Reference < XText > (xCell, UNO_QUERY);
        xTextCursor = xText -> createTextCursor();
        xCell = xTable -> getCellByName(OUString::createFromAscii(((char)('B') + std::to_string(i)).c_str()));
        xCell -> setValue(it -> second);
        i++;
    }
    return;

}

bool IsOnlyLatin(OUString curWord) {
    if (!curWord.getLength()) {
        return false;
    }
    for (std::size_t i = 0; i < curWord.getLength(); i++) {
        if (!std::isalpha(((unsigned char) curWord[i]))) {
            return false;
        }
    }
    return true;
}

bool MyIsAlphaAndDigit(char16_t letter) {
    return std::isalpha((unsigned char) letter) || cyrillic.find(letter) != std::string::npos || std::isdigit((unsigned char) letter);
}


void MakeRed(Reference < XFrame > & mxFrame) {
    Reference < XTextDocument > xTextDocument(mxFrame -> getController() -> getModel(), UNO_QUERY);
    Reference < XText > xText = xTextDocument -> getText();
    Reference < XTextCursor > xTextCursor = xText -> createTextCursor();
    std::size_t curSum = 0, curSize, textSize;
    textSize = (xText -> getString()).getLength();
    Reference < XPropertySet > xCursorProps(xTextCursor, UNO_QUERY);
    while (curSum < textSize && xTextCursor -> goRight(1, true)) {
        curSize = 0;
        while (curSize < (textSize - curSum) && MyIsAlphaAndDigit((xTextCursor -> getString())[curSize])) {
            xTextCursor -> goRight(1, true);
            curSize++;
        }
        if (curSize != (textSize - curSum)) {
            xTextCursor -> goLeft(1, true);

        }
        if (IsOnlyLatin(xTextCursor -> getString())) {
            xCursorProps -> setPropertyValue("CharBackColor", makeAny(0xFF0000));
        }
        if (xTextCursor -> goRight(1, true)) {
            xTextCursor -> collapseToEnd();
            curSum += curSize + 1;
        } else {
            break;
        }
    }
    return;
}