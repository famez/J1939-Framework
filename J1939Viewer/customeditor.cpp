#include <qevent.h>
#include "customeditor.h"

TreeViewCustomEditor::TreeViewCustomEditor()
{

}

void TreeViewCustomEditor::paintEvent(QPaintEvent *event) {

    /*
     * This is actually done because I do not know a better way to obtain the available region to paint and be able to adapt the content of the widget
     */


    if(mRegion != event->rect()) {
        mRegion = event->rect();
        regionToPaintChanged(event->rect());
    }

    QWidget::paintEvent(event);
}
