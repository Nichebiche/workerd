// Copyright (c) 2017-2022 Cloudflare, Inc.
// Licensed under the Apache 2.0 license found in the LICENSE file or at:
//     https://opensource.org/licenses/Apache-2.0

#pragma once

#include <workerd/io/compatibility-date.capnp.h>
#include <workerd/io/supported-compatibility-date.capnp.h>
#include <workerd/io/worker.h>

namespace workerd {

enum class CompatibilityDateValidation {
  // Allow dates up through the date specified by `supportedCompatibilityDate` in
  // `compatibility-date.capnp`.
  CODE_VERSION,

  // Allow dates up to through the current date.
  //
  // This should ONLY be used by Cloudflare. If Cloudflare fails to deploy a compatibility flag
  // before its default-on date passes, then the flag's default-on date needs to be pushed back.
  CURRENT_DATE_FOR_CLOUDFLARE,
  // TODO(someday): We may want to consider eliminating this and using CODE_VERSION even for
  //   Cloudflare. Once people are testing their code using the open source runtime in wrangler,
  //   they will be forced to set a date no later than `supportedCompatibilityDate` anyway. If
  //   we make sure Cloudflare is updated before the runtime used locally by Wrangler is updated,
  //   then there's no real need for Cloudflare to accept newer dates. This would then allow us
  //   to avoid the situation where a release misses the deadline implied by its compat flag
  //   default-on dates.

  // Allow any future date. This should only be used to test `compileCompatibilityFlags` itself.
  FUTURE_FOR_TEST
};

void compileCompatibilityFlags(kj::StringPtr compatDate, capnp::List<capnp::Text>::Reader compatFlags,
                         CompatibilityFlags::Builder output,
                         Worker::ValidationErrorReporter& errorReporter,
                         bool allowExperimentalFeatures,
                         CompatibilityDateValidation dateValidation);

// Return an array of compatibility enable-flags which express the given FeatureFlags. The returned
// StringPtrs point to FeatureFlags annotation parameters, which live in static storage.
kj::Array<kj::StringPtr> decompileCompatibilityFlagsForFl(CompatibilityFlags::Reader input);
// TODO(soon): Introduce a codec which can minimize the number of flags generated by choosing a good
//   compatibility date.

// Exposed to unit test the parser.
kj::Maybe<kj::String> normalizeCompatDate(kj::StringPtr date);

// Returns the current date as a string formatted by CompatDate.
kj::String currentDateStr();

// These values come from src/workerd/io/compatibility-date.capnp
static constexpr uint64_t COMPAT_ENABLE_FLAG_ANNOTATION_ID = 0xb6dabbc87cd1b03eull;
static constexpr uint64_t COMPAT_DISABLE_FLAG_ANNOTATION_ID = 0xd145cf1adc42577cull;
static constexpr uint64_t COMPAT_ENABLE_DATE_ANNOTATION_ID = 0x91a5d5d7244cf6d0ull;
static constexpr uint64_t COMPAT_ENABLE_ALL_DATES_ANNOTATION_ID = 0x9a1d37c8030d9418;
static constexpr uint64_t EXPERIMENTAl_ANNOTATION_ID = 0xe3e5a63e76284d88;
static constexpr uint64_t IMPLIED_BY_AFTER_DATE_ANNOTATION_ID = 0xe3e5a63e76284d89;
static constexpr uint64_t NEEDED_BY_FL = 0xbd23aff9deefc308ull;
static constexpr uint64_t PYTHON_SNAPSHOT_RELEASE_ANNOTATION_ID = 0xef74c0cc5d18cc0cull;

}  // namespace workerd
