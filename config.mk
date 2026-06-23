# Enable LTO when making a release build. Disable by setting to 0.
USE_LTO_ON_RELEASE ?= 1

# Keep the current Winston's Burrow prototype build focused on the playable slice.
# Set to 0 to restore the full expansion special table.
WINSTONS_BURROW_SLICE ?= 1
