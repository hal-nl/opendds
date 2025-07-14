/*
 * File:   DDSUDPClient.hpp
 *
 * Copyright (c) 2025 Haluk Ates
 * Licensed under the MIT License.
 *
 */

#ifndef DDSUDPCLIENT_HPP
#define DDSUDPCLIENT_HPP

#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

namespace DDSCOMMON
{

  /**
   * Class to be used as client for UDP connections
   */
  class DDSUDPClient
  {
  public:

    /**
     * Constructor for UDP client
     *
     * @param io_service
     * @param host
     * @param port
     */
    DDSUDPClient(boost::asio::io_service& io_service, const std::string& host, const std::string& port);
    virtual ~DDSUDPClient();

    /**
     * Method to send a string via UDP client
     *
     * @param msg
     */
    void Send(const std::string& msg);

  private:
    boost::asio::io_service&  io_service_;
    udp::socket               socket_;
    udp::endpoint             endpoint_;
  };

};

#endif /* DDSUDPCLIENT_HPP */

