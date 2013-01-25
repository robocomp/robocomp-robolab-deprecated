/*
 * Copyright (c) 2006-2007, Johan Thelin
 * 
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 * 
 *     * Redistributions of source code must retain the above copyright notice, 
 *       this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright notice,  
 *       this list of conditions and the following disclaimer in the documentation 
 *       and/or other materials provided with the distribution.
 *     * Neither the name of APress nor the names of its contributors 
 *       may be used to endorse or promote products derived from this software 
 *       without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
#ifndef OPENFILEDLG_H
#define OPENFILEDLG_H

#include <QApplication>
#include <QFileDialog>

class OpenFileDlg : public QWidget
{
public:
  QString openFile()
  {
//QFileDialog::getOpenFileName( this, tr("Open Document"), QDir::currentPath(), tr("Document files (*.txt *.rcd);;All files (*.*)"), 0, QFileDialog::DontUseNativeDialog );
    QString filename = QFileDialog::getOpenFileName( this, tr("Abrir Sesion"), QDir::currentPath(), tr("Archivos de sesion (*.txt *.rcd);;Todos los archivos (*.*)") );
    if( !filename.isNull() )
    {
//       qDebug( filename.toAscii() );
	  return filename;
    }
    else return 0;
  }
  /*
  void openFiles()
  {
    QStringList filenames = QFileDialog::getOpenFileNames( this, tr("Open Document"), QDir::currentPath(), tr("Documents (*.doc);;All files (*.*)") );
    if( !filenames.isEmpty() )
    {
//       qDebug( filenames.join(",").toAscii() );
    }
  }
  
  void openDir()
  {
    QString dirname = QFileDialog::getExistingDirectory( this, tr("Select a Directory"), QDir::currentPath() );
    if( !dirname.isNull() )
    {
//       qDebug( dirname.toAscii() );
    }
  }
  
  void saveFile()
  {
    QString filename = QFileDialog::getSaveFileName( this, tr("Save Document"), QDir::currentPath(), tr("Documents (*.txt)") );
    if( !filename.isNull() )
    {
//       qDebug( filename.toAscii() );
    }
  }*/
};

#endif