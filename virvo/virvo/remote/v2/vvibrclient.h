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

#ifndef VV_IBR_CLIENT_H
#define VV_IBR_CLIENT_H

#include "vvremoteclient.h"

class vvRenderer;
class vvVolDesc;

class vvIbrClient : public vvRemoteClient
{
public:
    typedef vvRemoteClient::MessagePtr MessagePtr;

public:
    VVAPI vvIbrClient(vvVolDesc *vd, vvRenderState renderState,
        virvo::ConnectionPointer conn, std::string const& filename = "");

    VVAPI virtual ~vvIbrClient();

    // vvRemoteClient API ------------------------------------------------------

    VVAPI virtual bool render() VV_OVERRIDE;

private:
    // TODO:
    // Move into Impl??
    void handler(virvo::Connection::Reason reason, virvo::MessagePointer message, boost::system::error_code const& e);

    void on_read(virvo::MessagePointer message);

    void on_write(virvo::MessagePointer message);

    void processIbrImage(virvo::MessagePointer message);

    void initIbrFrame();

private:
    struct Impl;
    boost::shared_ptr<Impl> impl_;
};

#endif
