/* ***** BEGIN LICENSE BLOCK *****
 * This file is part of Natron <http://www.natron.fr/>,
 * Copyright (C) 2016 INRIA and Alexandre Gauthier-Foichat
 *
 * Natron is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Natron is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Natron.  If not, see <http://www.gnu.org/licenses/gpl-2.0.html>
 * ***** END LICENSE BLOCK ***** */

#ifndef Engine_PyTracker_H
#define Engine_PyTracker_H

// ***** BEGIN PYTHON BLOCK *****
// from <https://docs.python.org/3/c-api/intro.html#include-files>:
// "Since Python may define some pre-processor definitions which affect the standard headers on some systems, you must include Python.h before any standard headers are included."
#include <Python.h>
// ***** END PYTHON BLOCK *****

#include "Global/Macros.h"

#include <list>

#if !defined(Q_MOC_RUN) && !defined(SBK_RUN)
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#endif

#include "Engine/PyParameter.h"
#include "Engine/EngineFwd.h"

NATRON_NAMESPACE_ENTER;
NATRON_PYTHON_NAMESPACE_ENTER;

class Track
{
public:

    Track(const boost::shared_ptr<TrackMarker>& marker);

    ~Track();

    boost::shared_ptr<TrackMarker> getInternalMarker() const
    {
        return _marker;
    }

    void setScriptName(const QString& scriptName);
    QString getScriptName() const;

    Param* getParam(const QString& scriptName) const;

    void reset();

private:

    boost::shared_ptr<TrackMarker> _marker;
};

class Tracker
{
public:

    Tracker(const boost::shared_ptr<TrackerContext>& ctx);

    ~Tracker();

    Track* getTrackByName(const QString& name) const;

    void getAllTracks(std::list<Track*>* markers) const;

    void getSelectedTracks(std::list<Track*>* markers) const;

    void startTracking(const std::list<Track*>& marks,
                       int start,
                       int end,
                       bool forward);

    void stopTracking();

private:

    boost::shared_ptr<TrackerContext> _ctx;
};

NATRON_PYTHON_NAMESPACE_EXIT;
NATRON_NAMESPACE_EXIT;

#endif // Engine_PyTracker_H
