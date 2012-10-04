// Virvo - Virtual Reality Volume Rendering
// Copyright (C) 1999-2003 University of Stuttgart, 2004-2005 Brown University
// Contact: Jurgen P. Schulze, jschulze@ucsd.edu
//
// This file is part of Virvo.
//
// Virvo is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library (see license.txt); if not, write to the
// Free Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA

#ifdef HAVE_CONFIG_H
#include "vvconfig.h"
#endif

#include "vvbonjoureventloop.h"

#include "vvdebugmsg.h"
#include "vvpthread.h"
#include "vvsocketmonitor.h"
#include "vvtcpsocket.h"

#ifdef HAVE_BONJOUR
#include <dns_sd.h>
#endif

namespace
{
#ifdef HAVE_BONJOUR
_DNSServiceRef_t *dnsServiceRef;
#endif
}

struct Thread
{
  Thread()
    : _pthread(NULL)
  {
  }

  pthread_t* _pthread;
};

vvBonjourEventLoop::vvBonjourEventLoop(void* service)
{
  _thread = new Thread;
  _thread->_pthread = NULL;
#ifdef HAVE_BONJOUR
  ::dnsServiceRef = reinterpret_cast<_DNSServiceRef_t*>(service);
#else
  (void)service;
#endif
}

vvBonjourEventLoop::~vvBonjourEventLoop()
{
  if (_thread != NULL && _thread->_pthread != NULL)
  {
    pthread_join(*_thread->_pthread, NULL);
    delete _thread->_pthread;
  }
  delete _thread;
}

void vvBonjourEventLoop::run(bool inThread, double timeout)
{
#ifdef HAVE_BONJOUR
  _timeout = timeout;
  _noMoreFlags = false;
  if(inThread == false)
  {
    loop(this);
    ::dnsServiceRef = NULL;
  }
  else
  {
    delete _thread->_pthread;
    _thread->_pthread = new pthread_t;
    pthread_create(_thread->_pthread, NULL, loop, this);
  }
#else
  (void)inThread;
  (void)timeout;
#endif
}

void * vvBonjourEventLoop::loop(void * attrib)
{
#ifdef HAVE_BONJOUR
  vvDebugMsg::msg(3, "vvBonjourEventLoop::loop()");
  vvBonjourEventLoop *instance = reinterpret_cast<vvBonjourEventLoop*>(attrib);

  instance->_run = true;
  int dns_sd_fd = DNSServiceRefSockFD(::dnsServiceRef);

  vvTcpSocket sock = vvTcpSocket();
  sock.setSockfd(dns_sd_fd);

  std::vector<vvSocket*> sockets;
  sockets.push_back(&sock);

  vvSocketMonitor socketMonitor;
  socketMonitor.setReadFds(sockets);

  while (instance->_run)
  {
    vvSocket *ready;
    double to = instance->_timeout;
    vvSocketMonitor::ErrorType smErr = socketMonitor.wait(&ready, &to);

    if (smErr == vvSocketMonitor::VV_OK)
    {
      DNSServiceErrorType err = DNSServiceProcessResult(::dnsServiceRef);

      switch(err)
      {
      case kDNSServiceErr_NoError:
        continue;
        break;
      case kDNSServiceErr_NoSuchName:
        vvDebugMsg::msg(1, "DNSServiceProcessResult() Given name does not exist");
        break;
      case kDNSServiceErr_NoMemory:
        vvDebugMsg::msg(1, "DNSServiceProcessResult() Out of memory");
        break;
      case kDNSServiceErr_BadParam:
        vvDebugMsg::msg(1, "DNSServiceProcessResult() Parameter contains invalid data");
        break;
      case kDNSServiceErr_BadReference:
        vvDebugMsg::msg(1, "DNSServiceProcessResult() Reference being passed is invalid");
        break;
      case kDNSServiceErr_BadState:
        vvDebugMsg::msg(1, "DNSServiceProcessResult() Internal error");
        break;
      case kDNSServiceErr_BadFlags:
        vvDebugMsg::msg(1, "DNSServiceProcessResult() Invalid values for flags");
        break;
      case kDNSServiceErr_Unsupported:
        vvDebugMsg::msg(1, "DNSServiceProcessResult() Operation not supported");
        break;
      case kDNSServiceErr_NotInitialized:
        vvDebugMsg::msg(1, "DNSServiceProcessResult() Reference not initialized");
        break;
      case kDNSServiceErr_AlreadyRegistered:
        vvDebugMsg::msg(1, "DNSServiceProcessResult() Attempt to register a service that is registered");
        break;
      case kDNSServiceErr_NameConflict:
        vvDebugMsg::msg(1, "DNSServiceProcessResult() Attempt to register a service with an already used name");
        break;
      case kDNSServiceErr_Invalid:
        vvDebugMsg::msg(1, "DNSServiceProcessResult() Certain invalid parameter data, such as domain name more than 255 bytes long");
        break;
      case kDNSServiceErr_Incompatible:
        vvDebugMsg::msg(1, "DNSServiceProcessResult() Client library incompatible with daemon");
        break;
      case kDNSServiceErr_BadInterfaceIndex:
        vvDebugMsg::msg(1, "DNSServiceProcessResult() Specified interface does not exist");
        break;
      case kDNSServiceErr_Unknown:
      default:
        vvDebugMsg::msg(1, "DNSServiceProcessResult() unknown error");
        break;
      }

      // did not "continue", so error occured -> break!
      break;
    }
    else if(smErr == vvSocketMonitor::VV_TIMEOUT)
    {
      vvDebugMsg::msg(1, "vvBonjourEventLoop::loop() timeout reached");
      break;
    }
    else
    {
      vvDebugMsg::msg(1, "vvBonjourEventLoop::loop() socketmonitor returned error");
      break;
    }
  }
#else
  (void)attrib;
#endif
  return NULL;
}

void vvBonjourEventLoop::stop()
{
  vvDebugMsg::msg(3, "vvBonjourEventLoop::stop()");
  _run = false;
}

// vim: sw=2:expandtab:softtabstop=2:ts=2:cino=\:0g0t0
