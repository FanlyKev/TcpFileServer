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
    connect(&tcpServer, SIGNAL(acceptError(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError));
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

}

void TcpFileServer::updateServerProgress()
{

}

void displayError(QAbstractSocket::SocketError socketError)
{

}