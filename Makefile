include config

.PHONY: clean realclean configure dist doc

build:
ifeq ($(PLATFORM), win32-mingw)
	(cd src; $(MAKE) debug) 
else
	(cd src; $(MAKE))
endif

configure:
	(cd src; $(QMAKE) -spec $(QMAKE_SPEC) wdspick.pro)

clean:
	(cd src; make clean)

realclean: 
	(cd src; make distclean)
	rm -f src/wdspick.app/Contents/MacOS/wdspick
	rm -f *~

doc:
	pandoc -o CHANGELOG.txt CHANGELOG.md
	pandoc -o README.txt README.md

dist:
ifeq ($(PLATFORM), macos)
	make macos-dist
else
ifeq ($(PLATFORM), win32-mingw)
	make win32-dist
endif
endif

MACOS_DIST=/tmp/wdspick

macos-dist:
	make macos-install
	make macos-installer

macos-install:
	@echo "** Installing in $(MACOS_DIST)"
	rm -rf $(MACOS_DIST)
	mkdir $(MACOS_DIST)
	cp -r src/wdspick.app $(MACOS_DIST)/Wdspick.app
	cp ./dist/macos/wdspick.ini $(MACOS_DIST)/Wdspick.app/Contents/MacOS
	cp ./dist/macos/INSTALL $(MACOS_DIST)
	mkdir $(MACOS_DIST)/doc
	cp  doc/um/WdsPickUserManual.pdf $(MACOS_DIST)/doc
	mkdir $(MACOS_DIST)/etc
	cp -r etc/* $(MACOS_DIST)/etc
	cp CHANGELOG.md LICENSE README.md $(MACOS_DIST)

WDSPICK_VOLUME=Wdspick-$(VERSION)

macos-installer:
	@echo "** Creating disk image"
	rm -f /tmp/Wdspick.dmg
	hdiutil create -size 32m -fs HFS+ -volname "$(WDSPICK_VOLUME)" /tmp/Wdspick.dmg
	hdiutil attach /tmp/Wdspick.dmg
	cp -r $(MACOS_DIST)/Wdspick.app /Volumes/$(WDSPICK_VOLUME)
	ln -s /Applications /Volumes/$(WDSPICK_VOLUME)/Applications
	cp -r $(MACOS_DIST)/etc /Volumes/$(WDSPICK_VOLUME)/Extra
	cp -r $(MACOS_DIST)/doc /Volumes/$(WDSPICK_VOLUME)/Documentation
	cp $(MACOS_DIST)/{CHANGELOG.md,LICENSE,README.md,INSTALL} /Volumes/$(WDSPICK_VOLUME)
	hdiutil detach /Volumes/$(WDSPICK_VOLUME)
	hdiutil convert /tmp/Wdspick.dmg -format UDZO -o /tmp/Wdspick_ro.dmg
	mv /tmp/Wdspick_ro.dmg /tmp/Wdspick.dmg
	@echo "** Done. Disk image is /tmp/Wdspick.dmg"

WIN_INSTALL_DIR=./dist/windows/build

win32-dist:
	make win32-install
	make win32-installer

win32-install:
#	cp ./src/release/wdspick.exe $(WIN_INSTALL_DIR)
	cp ./src/debug/wdspick.exe $(WIN_INSTALL_DIR)
	cp {CHANGELOG.md,LICENSE,README.md} $(WIN_INSTALL_DIR)
	cp  doc/um/WdsPickUserManual.pdf $(WIN_INSTALL_DIR)

win32-installer:
	/C/Program\ Files\ \(x86\)/Inno\ Setup\ 6/iscc ./dist/windows/WdspickSetup.iss
