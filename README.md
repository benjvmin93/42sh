BUILD SYSTEM

-To commpile it
    -meson setup builddir(name of the directory to put the executable)
    -ninja -C builddir

-Add a new directory with some new files and test it

    -First go in the new directory and create a meson.build. Add the files with this implementation:
        all_sources += files (
            'new files1',
            'new files2',
        )

    -Secondly exit from this directory with 'cd ..' and create or modify the local meson.build. Add in it the new directory with this implementation:
        subdir('new directory')
