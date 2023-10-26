#!/bin/bash

CONFIG=release

./_build/linux-x86_64/${CONFIG}/kit/omni.app.dev.sh \
    --no-window \
    --ext-folder ./_build/linux-x86_64/${CONFIG}/exts \
    --enable omni.kit.profiler.window \
    --enable omni.kit.browser.sample \
    --/app/fastShutdown=true \
    --/foundation/verifyOsVersion/enabled=false \
    --enable omni.jing.debug.toy
