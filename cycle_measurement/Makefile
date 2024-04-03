ifeq ($(strip $(DEVKITPPC)),)
$(error "In order to build this program, please install devkitPPC.")
endif

include $(DEVKITPPC)/wii_rules

# Build directory
BUILDDIR := build

# Output directory
OUTDIR := out

# Target filepath
TARGETPATH := $(OUTDIR)/cycle_measurement

# Source directory
SOURCEDIR := source

# Source files
CPPFILES := $(wildcard $(SOURCEDIR)/*.cpp)

# Object files
OBJECTFILES := $(CPPFILES:$(SOURCEDIR)/%.cpp=$(BUILDDIR)/%.o)

# Compiler flags
CXXFLAGS := -std=c++20 -O3 -Wall -Werror -Wextra $(MACHDEP) -Iinclude -I$(LIBOGC_INC)

# Linker editor
LD := $(CXX)

# Linker flags
LDFLAGS := $(MACHDEP)

# Libraries
LIBS := -lwiiuse -lbte -logc

# Library paths
LIBPATHS := -L$(LIBOGC_LIB) $(foreach dir,$(LIBDIRS),-L$(dir)/lib)

all: $(TARGETPATH).dol

.PHONY: clean
clean:
	@echo Cleaning...
	@rm -rf $(BUILDDIR) $(OUTDIR)

$(BUILDDIR)/%.o: $(SOURCEDIR)/%.cpp
	@mkdir -p $(BUILDDIR) $(OUTDIR)

	@echo Compiling $<...
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGETPATH).elf: $(OBJECTFILES)

$(TARGETPATH).dol: $(TARGETPATH).elf
