/* ***** BEGIN LICENSE BLOCK *****
 * This file is part of Natron <http://www.natron.fr/>,
 * Copyright (C) 2015 INRIA and Alexandre Gauthier-Foichat
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

#ifndef TRACKERCONTEXTPRIVATE_H
#define TRACKERCONTEXTPRIVATE_H

// ***** BEGIN PYTHON BLOCK *****
// from <https://docs.python.org/3/c-api/intro.html#include-files>:
// "Since Python may define some pre-processor definitions which affect the standard headers on some systems, you must include Python.h before any standard headers are included."
#include <Python.h>
// ***** END PYTHON BLOCK *****

#include <list>



#include <libmv/autotrack/autotrack.h>

#include <openMVG/robust_estimation/robust_estimator_Prosac.hpp>

#include "Engine/EngineFwd.h"
#include "Engine/RectD.h"

#include "Engine/TrackerContext.h"


#define kTrackBaseName "track"
#define TRACKER_MAX_TRACKS_FOR_PARTIAL_VIEWER_UPDATE 8

/// Parameters definitions

//////// Global to all tracks
#define kTrackerParamTrackRed "trackRed"
#define kTrackerParamTrackRedLabel "Track Red"
#define kTrackerParamTrackRedHint "Enable tracking on the red channel"

#define kTrackerParamTrackGreen "trackGreen"
#define kTrackerParamTrackGreenLabel "Track Green"
#define kTrackerParamTrackGreenHint "Enable tracking on the green channel"

#define kTrackerParamTrackBlue "trackBlue"
#define kTrackerParamTrackBlueLabel "Track Blue"
#define kTrackerParamTrackBlueHint "Enable tracking on the blue channel"

#define kTrackerParamMaxError "maxError"
#define kTrackerParamMaxErrorLabel "Max. Error"
#define kTrackerParamMaxErrorHint "This is the minimum necessary error between the final tracked " \
"position of the patch on the destination image and the reference patch needed to declare tracking success." \
"The error is 1 minus the normalized cross-correlation score."

#define kTrackerParamMaximumIteration "maxIterations"
#define kTrackerParamMaximumIterationLabel "Maximum iterations"
#define kTrackerParamMaximumIterationHint "Maximum number of iterations the algorithm will run for the inner minimization " \
"before it gives up."

#define kTrackerParamBruteForcePreTrack "bruteForcePretTrack"
#define kTrackerParamBruteForcePreTrackLabel "Use brute-force pre-track"
#define kTrackerParamBruteForcePreTrackHint "Use a brute-force translation-only pre-track before refinement"

#define kTrackerParamNormalizeIntensities "normalizeIntensities"
#define kTrackerParamNormalizeIntensitiesLabel "Normalize Intensities"
#define kTrackerParamNormalizeIntensitiesHint "Normalize the image patches by their mean before doing the sum of squared" \
" error calculation. Slower."

#define kTrackerParamPreBlurSigma "preBlurSigma"
#define kTrackerParamPreBlurSigmaLabel "Pre-blur sigma"
#define kTrackerParamPreBlurSigmaHint "The size in pixels of the blur kernel used to both smooth the image and take the image derivative."

#define kTrackerParamExportDataSeparator "exportDataSection"
#define kTrackerParamExportDataSeparatorLabel "Export"

#define kTrackerParamExportDataChoice "exportDataOptions"
#define kTrackerParamExportDataChoiceLabel "Type"
#define kTrackerParamExportDataChoiceHint "Select the desired option to the Transform/CornerPin node that will be exported. " \
"Each export has a \"baked\" option which will copy the animation out of the tracks instead of linking them directly."

#define kTrackerParamExportDataChoiceCPThisFrame "CornerPin (Use current frame), linked"
#define kTrackerParamExportDataChoiceCPThisFrameHint "Warp the image according to the relative transform using the current frame as reference"

#define kTrackerParamExportDataChoiceCPRefFrame "CornerPin (Reference frame), linked"
#define kTrackerParamExportDataChoiceCPRefFrameHint "Warp the image according to the relative transform using the reference frame selected in " \
" the Transform tab as reference"

#define kTrackerParamExportDataChoiceCPThisFrameBaked "CornerPin (Use current frame), baked"

#define kTrackerParamExportDataChoiceCPRefFrameBaked "CornerPin (Reference frame), baked"

#define kTrackerParamExportDataChoiceTransformStabilize "Transform (Stabilize), linked"
#define kTrackerParamExportDataChoiceTransformStabilizeHint "Creates a Transform node that will stabilize the footage. The Transform is identity " \
"at the reference frame selected in the Transform tab"

#define kTrackerParamExportDataChoiceTransformMatchMove "Transform (Match-move), linked"
#define kTrackerParamExportDataChoiceTransformMatchMoveHint "Creates a Transform node that will match-move the footage. The Transform is identity " \
"at the reference frame selected in the Transform tab"

#define kTrackerParamExportDataChoiceTransformStabilizeBaked "Transform (Stabilize), baked"
#define kTrackerParamExportDataChoiceTransformMatchMoveBaked "Transform (Match-move), baked"


#define kTrackerParamExportButton "export"
#define kTrackerParamExportButtonLabel "Export"
#define kTrackerParamExportButtonHint "Creates a node referencing the tracked data according to the export type chosen on the left"

#define kCornerPinInvertParamName "invert"

#define kTrackerParamTransformType "transformType"
#define kTrackerParamTransformTypeLabel "Transform Type"
#define kTrackerParamTransformTypeHint "The type of transform in output of this node."

#define kTrackerParamTransformTypeNone "None"
#define kTrackerParamTransformTypeNoneHelp "No transformation applied in output to the image: this node is a pass-through. Set it to this mode when tracking to correclty see the input image on the viewer"

#define kTrackerParamTransformTypeStabilize "Stabilize"
#define kTrackerParamTransformTypeStabilizeHelp "Transforms the image so that the tracked points do not move"

#define kTrackerParamTransformTypeMatchMove "Match-Move"
#define kTrackerParamTransformTypeMatchMoveHelp "Transforms a different image so that it moves to match the tracked points"

#define kTrackerParamTransformTypeRemoveJitter "Remove Jitter"
#define kTrackerParamTransformTypeRemoveJitterHelp "Transforms the image so that the tracked points move smoothly with high frequencies removed"

#define kTrackerParamTransformTypeAddJitter "Add Jitter"
#define kTrackerParamTransformTypeAddJitterHelp "Transforms the image by the high frequencies of the animation of the tracks to increase the shake or apply it on another image"

#define kTrackerParamReferenceFrame "referenceFrame"
#define kTrackerParamReferenceFrameLabel "Reference frame"
#define kTrackerParamReferenceFrameHint "When exporting tracks to a CornerPin or Transform, this will be the frame number at which the transform will be an identity."

#define kTrackerParamSetReferenceFrame "setReferenceButton"
#define kTrackerParamSetReferenceFrameLabel "Set To Current Frame"
#define kTrackerParamSetReferenceFrameHint "Set the reference frame to the timeline's current frame"

#define kTrackerParamJitterPeriod "jitterPeriod"
#define kTrackerParamJitterPeriodLabel "Jitter Period"
#define kTrackerParamJitterPeriodHint "Number of frames to average together to remove high frequencies for the add/remove jitter transform type"

#define kTrackerParamSmooth "smooth"
#define kTrackerParamSmoothLabel "Smooth"
#define kTrackerParamSmoothHint "Smooth the translation/rotation/scale by averaging this number of frames together"

#define kTransformParamTranslate "translate"
#define kTransformParamRotate "rotate"
#define kTransformParamScale "scale"
#define kTransformParamUniform "uniform"
#define kTransformParamSkewX "skewX"
#define kTransformParamSkewY "skewY"
#define kTransformParamSkewOrder "skewOrder"
#define kTransformParamCenter "center"
#define kTransformParamInvert "invert"
#define kTransformParamFilter "filter"
#define kTransformParamClamp "clamp"
#define kTransformParamBlackOutside "black_outside"
#define kTransformParamMotionBlur "motionBlur"
#define kTransformParamShutter "shutter"
#define kTransformParamShutterOffset "shutterOffset"
#define kTransformParamCustomShutterOffset "shutterCustomOffset"


NATRON_NAMESPACE_ENTER;


enum TrackerTransformTypeEnum
{
    eTrackerTransformTypeNone,
    eTrackerTransformTypeStabilize,
    eTrackerTransformTypeMatchMove,
    eTrackerTransformTypeRemoveJitter,
    eTrackerTransformTypeAddJitter
};

enum libmv_MarkerChannelEnum {
    LIBMV_MARKER_CHANNEL_R = (1 << 0),
    LIBMV_MARKER_CHANNEL_G = (1 << 1),
    LIBMV_MARKER_CHANNEL_B = (1 << 2),
};





struct TrackMarkerAndOptions
{
    TrackMarkerPtr natronMarker;
    mv::Marker mvMarker;
    mv::TrackRegionOptions mvOptions;
};




class TrackArgsLibMV
{
    int _start, _end;
    bool _isForward;
    boost::shared_ptr<TimeLine> _timeline;
    ViewerInstance* _viewer;
    boost::shared_ptr<mv::AutoTrack> _libmvAutotrack;
    boost::shared_ptr<TrackerFrameAccessor> _fa;
    std::vector<boost::shared_ptr<TrackMarkerAndOptions> > _tracks;
    
    //Store the format size because LibMV internally has a top-down Y axis
    double _formatWidth,_formatHeight;
    mutable QMutex _autoTrackMutex;
    
public:
    
    TrackArgsLibMV()
    : _start(0)
    , _end(0)
    , _isForward(false)
    , _timeline()
    , _viewer(0)
    , _libmvAutotrack()
    , _fa()
    , _tracks()
    , _formatWidth(0)
    , _formatHeight(0)
    {
        
    }
    
    TrackArgsLibMV(int start,
                   int end,
                   bool isForward,
                   const boost::shared_ptr<TimeLine>& timeline,
                   ViewerInstance* viewer,
                   const boost::shared_ptr<mv::AutoTrack>& autoTrack,
                   const boost::shared_ptr<TrackerFrameAccessor>& fa,
                   const std::vector<boost::shared_ptr<TrackMarkerAndOptions> >& tracks,
                   double formatWidth,
                   double formatHeight)
    : _start(start)
    , _end(end)
    , _isForward(isForward)
    , _timeline(timeline)
    , _viewer(viewer)
    , _libmvAutotrack(autoTrack)
    , _fa(fa)
    , _tracks(tracks)
    , _formatWidth(formatWidth)
    , _formatHeight(formatHeight)
    {
        
    }
    
    TrackArgsLibMV(const TrackArgsLibMV& other)
    {
        *this = other;
    }
    
    void operator=(const TrackArgsLibMV& other)
    {
        _start = other._start;
        _end = other._end;
        _isForward = other._isForward;
        _timeline = other._timeline;
        _viewer = other._viewer;
        _libmvAutotrack = other._libmvAutotrack;
        _fa = other._fa;
        _tracks = other._tracks;
        _formatWidth = other._formatWidth;
        _formatHeight = other._formatHeight;
    }
    
    double getFormatHeight() const
    {
        return _formatHeight;
    }
    
    double getFormatWidth() const
    {
        return _formatWidth;
    }
    
    QMutex* getAutoTrackMutex() const
    {
        return &_autoTrackMutex;
    }
    
    int getStart() const
    {
        return _start;
    }
    
    int getEnd() const
    {
        return _end;
    }
    
    bool getForward() const
    {
        return _isForward;
    }
    
    boost::shared_ptr<TimeLine> getTimeLine() const
    {
        return _timeline;
    }
    
    ViewerInstance* getViewer() const
    {
        return _viewer;
    }
    
    int getNumTracks() const
    {
        return (int)_tracks.size();
    }
    
    const std::vector<boost::shared_ptr<TrackMarkerAndOptions> >& getTracks() const
    {
        return _tracks;
    }
    
    boost::shared_ptr<mv::AutoTrack> getLibMVAutoTrack() const
    {
        return _libmvAutotrack;
    }
    
    void getEnabledChannels(bool* r, bool* g, bool* b) const;
    
    void getRedrawAreasNeeded(int time, std::list<RectD>* canonicalRects) const;
};



struct TrackerContextPrivate
{
    
    TrackerContext* _publicInterface;
    boost::weak_ptr<Natron::Node> node;
    
    std::list<boost::weak_ptr<KnobI> > knobs,perTrackKnobs;
    boost::weak_ptr<KnobBool> enableTrackRed,enableTrackGreen,enableTrackBlue;
    boost::weak_ptr<KnobDouble> maxError;
    boost::weak_ptr<KnobInt> maxIterations;
    boost::weak_ptr<KnobBool> bruteForcePreTrack,useNormalizedIntensities;
    boost::weak_ptr<KnobDouble> preBlurSigma;
    
    boost::weak_ptr<KnobSeparator> exportDataSep;
    boost::weak_ptr<KnobChoice> exportChoice;
    boost::weak_ptr<KnobButton> exportButton;
    
    NodePtr transformNode, cornerPinNode;
    
    boost::weak_ptr<KnobChoice> transformType;
    boost::weak_ptr<KnobInt> referenceFrame;
    boost::weak_ptr<KnobButton> setCurrentFrameButton;
    boost::weak_ptr<KnobInt> jitterPeriod;
    boost::weak_ptr<KnobInt> smoothTransform;
    boost::weak_ptr<KnobDouble> translate;
    boost::weak_ptr<KnobDouble> rotate;
    boost::weak_ptr<KnobDouble> scale;
    boost::weak_ptr<KnobBool> scaleUniform;
    boost::weak_ptr<KnobDouble> skewX;
    boost::weak_ptr<KnobDouble> skewY;
    boost::weak_ptr<KnobChoice> skewOrder;
    boost::weak_ptr<KnobDouble> center;
    boost::weak_ptr<KnobBool> invertTransform;
    boost::weak_ptr<KnobChoice> filter;
    boost::weak_ptr<KnobBool> clamp;
    boost::weak_ptr<KnobBool> blackOutside;
    boost::weak_ptr<KnobDouble> motionBlur;
    boost::weak_ptr<KnobDouble> shutter;
    boost::weak_ptr<KnobChoice> shutterOffset;
    boost::weak_ptr<KnobDouble> customShutterOffset;
    
    mutable QMutex trackerContextMutex;
    std::vector<TrackMarkerPtr > markers;
    std::list<TrackMarkerPtr > selectedMarkers,markersToSlave,markersToUnslave;
    int beginSelectionCounter;
    int selectionRecursion;
    
    TrackScheduler<TrackArgsLibMV> scheduler;
    
    
    TrackerContextPrivate(TrackerContext* publicInterface, const boost::shared_ptr<Natron::Node> &node);
    
    
    /// Make all calls to getValue() that are global to the tracker context in here
    void beginLibMVOptionsForTrack(mv::TrackRegionOptions* options) const;
    
    /// Make all calls to getValue() that are local to the track in here
    void endLibMVOptionsForTrack(const TrackMarker& marker,
                                 mv::TrackRegionOptions* options) const;
    
    void addToSelectionList(const TrackMarkerPtr& marker)
    {
        if (std::find(selectedMarkers.begin(), selectedMarkers.end(), marker) != selectedMarkers.end()) {
            return;
        }
        selectedMarkers.push_back(marker);
        markersToSlave.push_back(marker);
    }
    
    void removeFromSelectionList(const TrackMarkerPtr& marker)
    {
        std::list<TrackMarkerPtr >::iterator found = std::find(selectedMarkers.begin(), selectedMarkers.end(), marker);
        if (found == selectedMarkers.end()) {
            return;
        }
        selectedMarkers.erase(found);
        markersToUnslave.push_back(marker);
    }
    
    void incrementSelectionCounter()
    {
        ++beginSelectionCounter;
    }
    
    void decrementSelectionCounter()
    {
        if (beginSelectionCounter > 0) {
            --beginSelectionCounter;
        }
    }
    
    void linkMarkerKnobsToGuiKnobs(const std::list<TrackMarkerPtr >& markers,
                                   bool multipleTrackSelected,
                                   bool slave);
    
    void createCornerPinFromSelection(const std::list<TrackMarkerPtr > & selection,
                                      bool linked,
                                      bool useTransformRefFrame,
                                      bool invert);
    
    void createTransformFromSelection(const std::list<TrackMarkerPtr > & selection,
                                      bool linked,
                                      bool invert);
    
        
    static void natronTrackerToLibMVTracker(bool useRefFrameForSearchWindow,
                                            bool trackChannels[3],
                                            const TrackMarker& marker,
                                            int trackIndex,
                                            int time,
                                            bool forward,
                                            double formatHeight,
                                            mv::Marker* mvMarker);
    
    static void setKnobKeyframesFromMarker(const mv::Marker& mvMarker,
                                           int formatHeight,
                                           const libmv::TrackRegionResult* result,
                                           const TrackMarkerPtr& natronMarker);
    
    
    static bool
    trackStepLibMV(int trackIndex, const TrackArgsLibMV& args, int time);
    
    
};

NATRON_NAMESPACE_EXIT;

#endif // TRACKERCONTEXTPRIVATE_H