
#include "serialportboost.h"
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/interprocess/detail/atomic.hpp>


CSerialPortBoost::CSerialPortBoost( std::string com , uint32_t baudrate)
    :m_work_ptr( new boost::asio::io_service::work( m_io_service ) ),m_receive_buffer_size(4096),m_com_number(com) ,m_baudrate(baudrate)
{

}

CSerialPortBoost:: ~CSerialPortBoost()
{

}

boost::asio::io_service & CSerialPortBoost::GetService()
{
    return m_io_service;
}

void CSerialPortBoost::SetReceiveBufferSize( int32_t size )
{
    m_receive_buffer_size = size;
}

int32_t CSerialPortBoost::GetReceiveBufferSize() const
{
    return m_receive_buffer_size;
}

void CSerialPortBoost::Run(uint16_t num_thread )
{
    for (uint16_t i=0;i<num_thread;i++)
    {
        m_worker_threads.create_thread( boost::bind( &CSerialPortBoost::WorkThread,shared_from_this() ) );
    }

}

void CSerialPortBoost::WorkThread()
{
    m_io_service.run();
}

void CSerialPortBoost::Close()
{
        m_work_ptr.reset();

        m_io_service.stop();
        //
        m_worker_threads.join_all();
}

bool CSerialPortBoost::Open()
{
    try
    {
        m_serial_port_ptr = boost::shared_ptr< boost::asio::serial_port>(new boost::asio::serial_port(m_io_service, m_com_number));

        m_serial_port_ptr->set_option(boost::asio::serial_port::baud_rate(m_baudrate));
        m_serial_port_ptr->set_option(boost::asio::serial_port::flow_control());
        m_serial_port_ptr->set_option(boost::asio::serial_port::parity(boost::asio::serial_port::parity::even));
        m_serial_port_ptr->set_option(boost::asio::serial_port::stop_bits(boost::asio::serial_port::stop_bits::one));
        m_serial_port_ptr->set_option(boost::asio::serial_port::character_size(8));
    }
    catch (boost::system::system_error e)
    {
        return false;
    }

    Run();
    Recv();

    return true;
}

bool CSerialPortBoost::IsOpen()
{
    return m_serial_port_ptr->is_open();
}



void CSerialPortBoost::Recv( int32_t total_bytes )
{
    StartRecv(total_bytes);
}

void CSerialPortBoost::StartRecv( int32_t total_bytes )
{
    if( total_bytes > 0 )
    {
        m_recv_buffer.resize( total_bytes );
        boost::asio::async_read( *m_serial_port_ptr, boost::asio::buffer( m_recv_buffer ), boost::bind( &CSerialPortBoost::HandleRecv, shared_from_this(), _1, _2 ) ) ;
    }
    else
    {
        m_recv_buffer.resize( m_receive_buffer_size );
        m_serial_port_ptr->async_read_some( boost::asio::buffer( m_recv_buffer ),  boost::bind( &CSerialPortBoost::HandleRecv, shared_from_this(), _1, _2 ) ) ;
    }
}

void CSerialPortBoost::HandleRecv( const boost::system::error_code & error, int32_t actual_bytes )
{
// 	std::cout << GetCurrentThreadId()<<"[" << __FUNCTION__ << "] "<< std::endl;
    if( error  )
    {
        m_serial_port_ptr->close();
        m_io_service.reset();
        Open();
    }
    else
    {
        m_recv_buffer.resize( actual_bytes );
// 		OnRecv( m_recv_buffer );
        if (ParseDataFunc)
        {
            ParseDataFunc(m_recv_buffer);
        }
        m_recv_buffer.clear();
        StartRecv();
    }
}



void CSerialPortBoost::Send(const std::vector< uint8_t > & buffer)
{
    bool should_start_send = m_pending_sends.empty();
    m_pending_sends.push_back( buffer );
    if( should_start_send )
    {
        StartSend();
    }
}

void CSerialPortBoost::StartSend()
{
    if( !m_pending_sends.empty() )
    {
// 		boost::asio::async_write( *m_serial_port_ptr, boost::asio::buffer( m_pending_sends.front() ),  boost::bind( &CSerialPortBoost::HandleSend, shared_from_this() , boost::asio::placeholders::error, m_pending_sends.begin() )  );
        boost::asio::write(*m_serial_port_ptr, boost::asio::buffer(m_pending_sends.front()));
        m_pending_sends.pop_front();
        StartSend();
    }
}

void CSerialPortBoost::HandleSend( const boost::system::error_code & error, std::list< std::vector< uint8_t > >::iterator itr )
{
    if( error)
    {
// 		StartError( error );
    }
    else
    {
        OnSend( *itr );
        m_pending_sends.erase( itr );
        StartSend();
    }
}

void  CSerialPortBoost::OnRecv( std::vector< uint8_t > & buffer )
{
    std::cout << "[" << __FUNCTION__ << "] " << buffer.size() << " bytes" << std::endl;
    std::cout << std::endl;
    //TODO:  handle the received data

}

void CSerialPortBoost::OnSend( const std::vector< uint8_t > & buffer )
{
    std::cout << "[" << __FUNCTION__ << "] " << buffer.size() << " bytes" << std::endl;
    std::cout << std::endl;
    //TODO:

}

