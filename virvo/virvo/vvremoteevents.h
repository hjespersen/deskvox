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

#ifndef _VVREMOTEEVENTS_H_
#define _VVREMOTEEVENTS_H_

namespace virvo
{
  enum RemoteEvent
  {
    // initialization
    Volume = 0,
    VolumeFile,

    // renderer parameters
    ParameterBool,
    ParameterInt32,
    ParameterInt64,
    ParameterUint32,
    ParameterUint64,
    ParameterFloat,
    ParameterVec3,
    ParameterVec4,
    ParameterSize3,
    ParameterColor,
    ParameterAABBI,
    CurrentFrame,
    ObjectDirection,
    ViewingDirection,
    Position,
    TransFunc,

    // rendering
    CameraMatrix,

    // misc.
    ServerInfo,
    GpuInfo,
    Statistics,
    WindowResize,
    RemoteServerType,
    Disconnect
  };
}

#endif // _VVREMOTEEVENTS_H_

//===================================================================
// End of File
//===================================================================
// vim: sw=2:expandtab:softtabstop=2:ts=2:cino=\:0g0t0
