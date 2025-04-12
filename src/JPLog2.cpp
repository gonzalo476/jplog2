/*
  JPLog2 Plugin for Nuke
  ------------------------------
  Copyright (c) 2025 Gonzalo Rojas
  This plugin is free to use, modify, and distribute.
  Provided "as is" without any warranty.

  https://github.com/NatronGitHub/openfx-misc/blob/master/Log2Lin/Log2Lin.cpp
*/

#include "include/JPLog2.h"

#include <algorithm>
#include <cmath>
#include <iostream>

#include <DDImage/Channel.h>
#include <DDImage/Knobs.h>
#include <DDImage/NukeWrapper.h>
#include <DDImage/PixelIop.h>
#include <DDImage/Row.h>

enum { LOGTOLIN = 0, LINTOLOG };

const char *JPLog2Iop::modes[] = {"jplog2 to lin", "lin to jplog2", nullptr};

JPLog2Iop::JPLog2Iop(Node *n) : PixelIop(n) {
  modeindex = 1;

  for (int i = 0; i < 3; ++i) {
    lin_breakpoint[i] = 0.00680f;
    log_breakpoint[i] = 0.16129f;
    slope[i] = 10.367739f;
    yint[i] = 0.0907775f;
  }
}

JPLog2Iop::~JPLog2Iop() {}

void JPLog2Iop::knobs(Knob_Callback f) {
  Enumeration_knob(f, &modeindex, modes, "operation", "operation");
  Color_knob(f, lin_breakpoint, IRange(0, 1), "linBreakpoint");
  // SetFlags(f, Knob::STORE_INTEGER);
  Color_knob(f, log_breakpoint, IRange(0, 1), "logBreakpoint");
  // SetFlags(f, Knob::STORE_INTEGER);
  Color_knob(f, slope, IRange(0, 1024), "slope");
  Color_knob(f, yint, IRange(0, 1), "yint");
}

void JPLog2Iop::_validate(bool for_real) {
  copy_info(0);
  set_out_channels(Mask_RGB);
}

void JPLog2Iop::in_channels(int, ChannelSet &mask) const {
  ChannelSet done;
  foreach (c, mask) {
    if (colourIndex(c) < 3 && !(done & c)) {
      done.addBrothers(c, 3);
    }
  }
  mask += done;
}

/* Functions */

static inline float jplog2ToLin(float v, float lin_breakpoint,
                                float log_breakpoint, float slope, float yint) {
  if (v <= log_breakpoint) {
    return (v - yint) / slope;
  } else {
    return std::powf(2.0f, v * 20.46f - 10.5f);
  }
}

static inline float LinToJplog2(float v, float lin_breakpoint,
                                float log_breakpoint, float slope, float yint) {
  if (v <= lin_breakpoint) {
    return slope * v + yint;
  } else {
    return (std::logf(v) / std::logf(2.0f) + 10.5f) / 20.46f;
  }
}

void JPLog2Iop::pixel_engine(const Row &in, int rowY, int rowX, int rowXBound,
                             ChannelMask outputChannels, Row &out) {
  int rowWidth = rowXBound - rowX;

  ChannelSet done;

  foreach (z, outputChannels) {

    if (done & z) {
      continue;
    }

    if (colourIndex(z) >= 3) {
      out.copy(in, z, rowX, rowXBound);
      continue;
    }

    Channel rChannel = brother(z, 0);
    Channel gChannel = brother(z, 1);
    Channel bChannel = brother(z, 2);

    done += rChannel;
    done += gChannel;
    done += bChannel;

    const float *rIn = in[rChannel] + rowX;
    const float *gIn = in[gChannel] + rowX;
    const float *bIn = in[bChannel] + rowX;

    float *rOut = out.writable(rChannel) + rowX;
    float *gOut = out.writable(gChannel) + rowX;
    float *bOut = out.writable(bChannel) + rowX;

    if (rOut != rIn)
      memcpy(rOut, rIn, sizeof(float) * rowWidth);
    if (gOut != gIn)
      memcpy(gOut, gIn, sizeof(float) * rowWidth);
    if (bOut != bIn)
      memcpy(bOut, bIn, sizeof(float) * rowWidth);

    const float *END = rIn + (rowXBound - rowX);

    switch (modeindex) {
    case LOGTOLIN:
      while (rIn < END) {
        *rOut++ = jplog2ToLin(*rIn++, lin_breakpoint[0], log_breakpoint[0],
                              slope[0], yint[0]);
        *gOut++ = jplog2ToLin(*gIn++, lin_breakpoint[1], log_breakpoint[1],
                              slope[1], yint[1]);
        *bOut++ = jplog2ToLin(*bIn++, lin_breakpoint[2], log_breakpoint[2],
                              slope[2], yint[2]);
      }
      break;
    case LINTOLOG:
      while (rIn < END) {
        *rOut++ = LinToJplog2(*rIn++, lin_breakpoint[0], log_breakpoint[0],
                              slope[0], yint[0]);
        *gOut++ = LinToJplog2(*gIn++, lin_breakpoint[1], log_breakpoint[1],
                              slope[1], yint[1]);
        *bOut++ = LinToJplog2(*bIn++, lin_breakpoint[2], log_breakpoint[2],
                              slope[2], yint[2]);
      }
      break;
    default:
      break;
    }
  }
}

const Op::Description JPLog2Iop::description("JPLog2", build);

const char *JPLog2Iop::Class() const { return description.name; }

const char *JPLog2Iop::displayName() const { return description.name; }

const char *JPLog2Iop::node_help() const { return "JPLog2 v1.0"; }

Op *build(Node *node) {
  NukeWrapper *op = new NukeWrapper(new JPLog2Iop(node));
  op->channels(Mask_RGB);
  return op;
}