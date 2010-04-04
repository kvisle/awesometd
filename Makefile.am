SUBDIRS = src

AWESOMETD       = awesometd$(EXEEXT)

CLEANFILES = $(AWESOMETD)

all-local: $(AWESOMETD)

EXTRA_DIST = $(shell find $(top_srcdir)/share)
$(AWESOMETD):
	$(LN_S) "src/$(AWESOMETD)" "$(AWESOMETD)"

install-data-hook:
	$(MKDIR_P) $(DESTDIR)$(datadir)/awesometd/textures
	$(INSTALL_DATA) -t $(DESTDIR)$(datadir)/awesometd/textures $(top_srcdir)/share/textures/*
	$(MKDIR_P) $(DESTDIR)$(datadir)/awesometd/fonts
	$(INSTALL_DATA) -t $(DESTDIR)$(datadir)/awesometd/fonts $(top_srcdir)/share/fonts/*
	$(MKDIR_P) $(DESTDIR)$(datadir)/awesometd/levels
	$(INSTALL_DATA) -t $(DESTDIR)$(datadir)/awesometd/levels $(top_srcdir)/share/levels/*

