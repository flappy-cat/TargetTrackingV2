#pragma once

//-----------------------------------------------------------------------------

#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <string>
#include <vector>
#include <list>
#include <boost/cstdint.hpp>
#include <boost/thread.hpp>
#include <boost/function.hpp>
//-----------------------------------------------------------------------------

using boost::uint64_t;
using boost::uint32_t;
using boost::uint16_t;
using boost::uint8_t;

using boost::int64_t;
using boost::int32_t;
using boost::int16_t;
using boost::int8_t;

class CSerialPortBoost;
typedef boost::shared_ptr< CSerialPortBoost > CSerialPortBoost_PTR;

class CSerialPortBoost : public boost::enable_shared_from_this< CSerialPortBoost >
{
public:
    CSerialPortBoost(std::string com , uint32_t baudrate);
    virtual ~CSerialPortBoost();

    // Returns the io_service of this object.
    boost::asio::io_service & GetService();

    // Sets the application specific receive buffer size used.
    // The default value is 1kb.
    void SetReceiveBufferSize( int32_t size );

    // Returns the size of the receive buffer size of the current object.
    int32_t GetReceiveBufferSize() const;

    //open a serial port and receive data from the serial port immediately
    bool Open();

    // the connection status of a serial port
    bool IsOpen();

    //close the serial port
    void Close();

    //write the buffer to a serial port
    void Send(const std::vector< uint8_t > & buffer);

    // Called when data has been sent
    void OnSend( const std::vector< uint8_t > & buffer );

    // Called when data has been received .You can handle the received data .
    void OnRecv( std::vector< uint8_t > & buffer );

    // Function defined by user to deal with data received from serial port, the callback function member have to be set before the serial port open
    boost::function< bool(std::vector< uint8_t >) > ParseDataFunc;

private:
    boost::asio::io_service m_io_service;
    boost::shared_ptr< boost::asio::io_service::work > m_work_ptr;
    boost::thread_group m_worker_threads;

    boost::shared_ptr< boost::asio::serial_port> m_serial_port_ptr;
    std::vector< uint8_t > m_recv_buffer;
    std::list< int32_t > m_pending_recvs;
    std::list< std::vector< uint8_t > > m_pending_sends;
    int32_t m_receive_buffer_size;

private:
    std::string m_com_number ;
    uint32_t m_baudrate ;

private:
    void Run(uint16_t num_thread = 1 );
    void WorkThread();

private:
    void Recv( int32_t total_bytes = 0 );
    void StartRecv( int32_t total_bytes=0 );
    void HandleRecv( const boost::system::error_code & error, int32_t actual_bytes );

    void StartSend();
    void HandleSend( const boost::system::error_code & error, std::list< std::vector< uint8_t > >::iterator itr );



};
