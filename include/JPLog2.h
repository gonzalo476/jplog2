/*
  JPLog2 Plugin for Nuke
  ------------------------------
  Copyright (c) 2025 Gonzalo Rojas
  This plugin is free to use, modify, and distribute.
  Provided "as is" without any warranty.
*/

#ifndef JPLOG2_H
#define JPLOG2_H

#include <DDImage/Channel.h>
#include <DDImage/Knobs.h>
#include <DDImage/NukeWrapper.h>
#include <DDImage/PixelIop.h>
#include <DDImage/Row.h>

using namespace DD::Image;

class JPLog2Iop : public PixelIop {
  int modeindex;
  float lin_breakpoint[3];
  float log_breakpoint[3];
  float slope[3];
  float yint[3];

public:
  static const char *modes[];

  JPLog2Iop(Node *node);

  ~JPLog2Iop() override;

  static const DD::Image::Op::Description description;

  const char *Class() const override;

  const char *displayName() const override;

  const char *node_help() const override;

  void knobs(Knob_Callback f) override;

  void in_channels(int n, ChannelSet &mask) const override;

  void pixel_engine(const Row &in, int rowY, int rowX, int rowXBound,
                    ChannelMask outputChannels, Row &out) override;

  void _validate(bool for_real) override;
};

static DD::Image::Op *build(Node *node);

#endif // JPLOG2_H