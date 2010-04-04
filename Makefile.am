SUBDIRS = src

AWESOMETD       = awesometd$(EXEEXT)

CLEANFILES = $(AWESOMETD)

all-local: $(AWESOMETD)

$(AWESOMETD):
	$(LN_S) "src/$(AWESOMETD)" "$(AWESOMETD)"

install-data-hook:
	$(MKDIR_P) $(datadir)/awesometd/textures
	$(INSTALL_DATA) -t $(datadir)/awesometd/textures $(top_srcdir)/share/textures/*
	$(MKDIR_P) $(datadir)/awesometd/fonts
	$(INSTALL_DATA) -t $(datadir)/awesometd/fonts $(top_srcdir)/share/fonts/*
	$(MKDIR_P) $(datadir)/awesometd/levels
	$(INSTALL_DATA) -t $(datadir)/awesometd/levels $(top_srcdir)/share/levels/*

