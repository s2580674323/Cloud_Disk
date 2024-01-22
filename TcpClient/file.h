#ifndef FILE_H
#define FILE_H

#include <QWidget>
#include <QListWidget>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>

#include <QInputDialog>
#include <QFileDialog>
#include <QFile>
#include <QTimer>

#include <QThread>

#include "protocol.h"
#include "sharefile.h"
#include "progressbar.h"
#include "uploadwork.h"
#include "downloadwork.h"


class File : public QWidget
{
    Q_OBJECT
public:
    explicit File(QWidget *parent = nullptr);

    void updateFileList(const PDU *pdu);
    QString getNextDirectory();
    void updateDirectoryPath(QString dirpath); // 更新QLabel显示的路径
    QString getSaveFilePath();
    void updateDownLoadStatus();
    QString getShareFileName();
    void updateSaveFilePath(QString SaveFilePath);

    // 请求服务器中，CurPath路径下的fileName文件，保存到本地的m_strSaveFilePath(包含文件名)路径下
    void DownLoadFile(QString strCurPath, QString fileName, QString strSaveFilePath);

signals:
    // 上传文件
    void SendFilePath_UpLoad(QString filepath);
    void SendCurPath_UpLoad(QString curpath);
    void SendFileName_UpLoad(QString filename);
    void SendFileSize_UpLoad(qint64 filesize);
    void StartConnect_UpLoad(unsigned int port, QString ip);

    // 下载文件
    void SendFileName_DownLoad(QString filename);
    void SendCurPath_DownLoad(QString curpath);
    void SendSaveFilePath_DownLoad(QString savefilepath);

    void StartConnect_DownLoad(unsigned int port, QString ip);


private slots:
    void clickedCreateDirBtn();
    void clickedDeleteDirBtn();
    void clickedRenameDirBtn();
    void clickedBackBtn();
    void clickedFlushScreenBtn();

    void doubleClickedItem();

    void clickedUpLoadFileBtn();
    void ScheduledDataSending(); // 定时发送上传文件的数据
    void clickedDeleteFileBtn();
    void clickedDownLoadFileBtn();
    void clickedShareFileBtn();
    void clickedMoveFileBtn();
    void clickedMoveFileConfirmBtn();

    // 上传文件

private:
    QListWidget *m_pFileListW;

    QPushButton *m_pBackBtn;
    QPushButton *m_pCreateDirBtn;
    QPushButton *m_pDeleteDirBtn;
    QPushButton *m_pReNameDirBtn;
    QPushButton *m_pFlushDirBtn;
    QPushButton *m_pUpLoadFileBtn;
    QPushButton *m_pDeleteFileBtn;
    QPushButton *m_pDownLoadFileBtn;
    QPushButton *m_pShareFileBtn;
    QPushButton *m_pMoveFileBtn;
    QPushButton *m_pMoveFileConfirmBtn;

    QLabel *m_pShowFileName;
    QLabel *m_pShowDirPath;
    QLabel *m_pDirPath;

    QString m_NextDirectory; // 每次请求进入文件夹，就保存一下要进入的文件夹的路径，不管是否进入成功

    QString m_strFilePath; // 保存 上传文件时，选中的本地文件的路径
    QTimer *m_pTimer; // 延迟发送，减少上传文件时粘包的可能性

    QString m_strSaveFilePath; // 下载文件的路径

    QString m_strMoveFileName; // 保存要移动的文件名
    QString m_strMoveFilePath; // 要移动的文件的路径
    QString m_strMoveDestDirPath; // 目标文件夹路径

    QString m_strShareFileName; // 要分享的文件名
};

#endif // FILE_H
