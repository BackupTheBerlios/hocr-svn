/***************************************************************************
 *            hocr_qt.cpp
 *
 *  Sun Aug 21 22:30:16 2005
 *  Copyright  2005  Yaacov Zamir
 *  <kzamir@walla.co.il>
 ****************************************************************************/

// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Library General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

#include <QtGui>
#include <hocrpp.h>

#include "hocr_qt.h"

hocr_qt::hocr_qt ()
{
	imageLabel = new QLabel;
	imageLabel->setBackgroundRole (QPalette::Base);
	imageLabel->setSizePolicy (QSizePolicy::Ignored,
				   QSizePolicy::Ignored);
	imageLabel->setScaledContents (true);

	scrollArea = new QScrollArea;
	scrollArea->setBackgroundRole (QPalette::Dark);
	scrollArea->setWidget (imageLabel);

	textEdit = new QTextEdit;
	textEdit->setLayoutDirection( Qt::RightToLeft );
	
	createActions ();
	createToolBars ();

	setCentralWidget (scrollArea);

	dockedText = new QDockWidget;
	dockedText->setWidget (textEdit);
	dockedText->
		setFeatures (QDockWidget::DockWidgetMovable | QDockWidget::
			     DockWidgetFloatable);
	addDockWidget (Qt::BottomDockWidgetArea, dockedText);

	setWindowTitle (tr ("HOCR-QT"));
	resize (700, 600);
}

void
hocr_qt::open ()
{
	QString fileName = QFileDialog::getOpenFileName (this,
							 tr ("Open File"),
							 QDir::
							 currentPath ());
	if (!fileName.isEmpty ())
	{
		QImage image (fileName);
		if (image.isNull ())
		{
			QMessageBox::information (this, tr ("HOCR-QT"),
						  tr ("Cannot load %1.").
						  arg (fileName));
			return;
		}
		setWindowTitle (tr ("HOCR-QT - %1").arg (fileName));
		imageLabel->setPixmap (QPixmap::fromImage (image));
		scaleFactor = 1.0;

		imageLabel->adjustSize ();
	}
}

void
hocr_qt::zoomIn ()
{
	scaleImage (1.25);
}

void
hocr_qt::zoomOut ()
{
	scaleImage (0.8);
}

void
hocr_qt::normalSize ()
{
	imageLabel->adjustSize ();
	scaleFactor = 1.0;
}

void
hocr_qt::about ()
{
	QMessageBox::about (this, tr ("About hocr-qt"),
			    tr
			    ("<p>HOCR-QT - Hebrew character recognition software<br>http://hocr.berlios.de<br>\
This project is supported by a grant form the Israeli Internet Association.<br>\
Copyright \xa9 2005 Yaacov Zamir.</p>"));
}

void
hocr_qt::createActions ()
{
	openAct = new QAction (tr ("&Open..."), this);
	openAct->setShortcut (tr ("Ctrl+O"));
	connect (openAct, SIGNAL (triggered ()), this, SLOT (open ()));

	applyAct = new QAction (tr ("&Apply"), this);
	applyAct->setShortcut (tr ("Ctrl+A"));
	connect (applyAct, SIGNAL (triggered ()), this, SLOT (apply ()));

	saveAct = new QAction (tr ("&Save"), this);
	saveAct->setShortcut (tr ("Ctrl+S"));
	connect (saveAct, SIGNAL (triggered ()), this, SLOT (save ()));

	zoomInAct = new QAction (tr ("Zoom &In (25%)"), this);
	zoomInAct->setShortcut (tr ("Ctrl++"));
	connect (zoomInAct, SIGNAL (triggered ()), this, SLOT (zoomIn ()));

	zoomOutAct = new QAction (tr ("Zoom &Out (25%)"), this);
	zoomOutAct->setShortcut (tr ("Ctrl+-"));
	connect (zoomOutAct, SIGNAL (triggered ()), this, SLOT (zoomOut ()));

	normalSizeAct = new QAction (tr ("&Normal Size"), this);
	normalSizeAct->setShortcut (tr ("Ctrl+S"));
	connect (normalSizeAct, SIGNAL (triggered ()), this,
		 SLOT (normalSize ()));

	aboutAct = new QAction (tr ("A&bout"), this);
	aboutAct->setShortcut (tr ("Ctrl+B"));
	connect (aboutAct, SIGNAL (triggered ()), this, SLOT (about ()));

	exitAct = new QAction (tr ("E&xit"), this);
	exitAct->setShortcut (tr ("Ctrl+Q"));
	connect (exitAct, SIGNAL (triggered ()), this, SLOT (close ()));
}

void
hocr_qt::createToolBars ()
{
	fileToolBar = addToolBar (tr ("File"));
	fileToolBar->addAction (openAct);
	fileToolBar->addAction (applyAct);
	fileToolBar->addAction (saveAct);

	zoomToolBar = addToolBar (tr ("Zoom"));

	zoomToolBar->addAction (zoomInAct);
	zoomToolBar->addAction (zoomOutAct);
	zoomToolBar->addAction (normalSizeAct);

	exitToolBar = addToolBar (tr ("Exit"));

	exitToolBar->addAction (aboutAct);
	exitToolBar->addAction (exitAct);
}

void
hocr_qt::scaleImage (double factor)
{
	if (!imageLabel->pixmap ())
		return;

	scaleFactor *= factor;
	imageLabel->resize (scaleFactor * imageLabel->pixmap ()->size ());

	adjustScrollBar (scrollArea->horizontalScrollBar (), factor);
	adjustScrollBar (scrollArea->verticalScrollBar (), factor);

	zoomInAct->setEnabled (scaleFactor < 3.0);
	zoomOutAct->setEnabled (scaleFactor > 0.333);
}

void
hocr_qt::adjustScrollBar (QScrollBar * scrollBar, double factor)
{
	scrollBar->setValue (int (factor * scrollBar->value ()
				  +
				  ((factor -
				    1) * scrollBar->pageStep () / 2)));
}

void
hocr_qt::apply ()
{
	if (!imageLabel->pixmap ())
		return;

	QImage img = imageLabel->pixmap ()->toImage ();

	{
		using namespace hocr;

		// create a new empty hocr object
		Hocr *hocr_engine = new Hocr;
		
		// fill the new hocr object with data from image window		
		hocr_engine->set_pixels ((char*)img.bits ()); // a pointer ! do not free it !
		hocr_engine->set_height (img.size ().height ());
		hocr_engine->set_width (img.size ().width ());
		hocr_engine->set_rowstride (img.bytesPerLine ());
		hocr_engine->set_n_channels (img.depth () / 8);
		hocr_engine->set_brightness (100);

		// get text into the text edit window
		textEdit->setPlainText( QString::fromUtf8(hocr_engine->do_ocr ()) );
		
		// if not set_pixels to 0 the next <free (hocr)> 
		// will free the original image window picture !
		hocr_engine->set_pixels (0);
		
		// free the hocr object
		free (hocr_engine);
	}
}

void
hocr_qt::save ()
{
	QString fileName = QFileDialog::getSaveFileName (this);
	if (fileName.isEmpty ())
		return;

	saveFile (fileName);
}

bool hocr_qt::saveFile (const QString & fileName)
{
	QFile
	file (fileName);
	if (!file.open (QFile::WriteOnly | QFile::Text))
	{
		QMessageBox::warning (this, tr ("Application"),
				      tr ("Cannot write file %1:\n%2.").
				      arg (fileName).arg (file.
							  errorString ()));
		return false;
	}

	QTextStream
	out (&file);
	QApplication::setOverrideCursor (Qt::WaitCursor);
	out << textEdit->toPlainText ();
	QApplication::restoreOverrideCursor ();

	return true;
}
