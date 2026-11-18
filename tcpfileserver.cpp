#include "tcpfileserver.h"

TcpFileServer::TcpFileServer(QWidget *parent)
    : QDialog(parent)
{
    totalBytes = 0;
    byteRecieved = 0;
    fileNameSize = 0;
    serverProgressBar = new QProgressBar;
    serverStatusLabel = new QLabel(tr("伺服器就緒"));
    startButton = new QPushButton("接收");
    quitButton = new QPushButton("推出");
    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(startButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(quitButton,QDialogButtonBox::RejectRole);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(serverProgressBar);
    mainLayout->addWidget(serverStatusLabel);
    mainLayout->addStretch();
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);
    setWindowTitle(tr("接收檔案"));
    connect(startButton, SIGNAL(clicked()), this, SLOT(start()));
    connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(&tcpServer, SIGNAL(newConnection()), this, SLOT(acceptConnection()));
    connect(&tcpServer, SIGNAL(acceptError(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));
}

TcpFileServer::~TcpFileServer()
{

}

void TcpFileServer::start()
{
    startButton->setEnabled(false);
    byteRecieved = 0;
    fileNameSize = 0;
    while(!tcpServer.isListening() && !tcpServer.listen(QHostAddress::AnyIPv4, 16689)){
        QMessageBox::StandardButton ret = QMessageBox::critical(this, tr("迴圈"), tr("無法啟動伺服器: %1.").arg(tcpServer.errorString()), QMessageBox::Retry | QMessageBox::Cancel);
    }
    serverStatusLabel->setText(tr("監聽中..."));
}

void TcpFileServer::acceptConnection()
{
    tcpServerConnection = tcpServer.nextPendingConnection();
    connect(tcpServerConnection, SIGNAL(readyRead()), this, SLOT(updateServerProgress()));
    connect(tcpServerConnection, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));
    serverStatusLabel->setText(tr("接受連線"));
    tcpServer.close();
}

void TcpFileServer::updateServerProgress()
{
    QDataStream in(tcpServerConnection);
    in.setVersion(QDataStream::Qt_4_6);
    if(byteRecieved <= sizeof(qint64) * 2)
    {
        if((fileNameSize == 0) && (tcpServerConnection->bytesAvailable() >= sizeof(qint64) * 2))
        {
            in >> totalBytes >> fileNameSize;
            byteRecieved += sizeof(qint64) * 2;
        }
        if((fileNameSize != 0) && (tcpServerConnection->bytesAvailable() >= fileNameSize))
        {
            in >> fileName;
            byteRecieved += fileNameSize;
            localFile = new QFile(fileName);
            if(!localFile->open(QFile::WriteOnly))
            {
                QMessageBox::warning(this, tr("應用程式"), tr("無法讀取檔案 %1 : \n%2.").arg(fileName).arg(localFile->errorString()));
                return;
            }
        } else{
            return;
        }
    }
}

void displayError(QAbstractSocket::SocketError socketError)
{


}