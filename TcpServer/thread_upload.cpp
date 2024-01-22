#include "thread_upload.h"

Thread_UpLoad::Thread_UpLoad()
{

    connect()
}

void Thread_UpLoad::run()
{
    QByteArray buffer = m_TcpSocket->readAll();
    m_file.write(buffer);
    m_lReceiveSize += buffer.size();
    if(m_lReceiveSize >= m_lTotalSize) {
        m_file.close();
    }

}
