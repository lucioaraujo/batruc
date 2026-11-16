# SPDX-License-Identifier: AGPL-3.0-or-later
# Copyright (c) 2026 Lúcio de Araújo

cmake_minimum_required(VERSION 3.22)

# Preparação para publicação (13/11/2026), no padrão já validado pelo Rasgo
# Modular, pelo ANTITOTEM e pelo Navalha 2:
# - Windows: runtime do Visual C++ ESTÁTICO (/MT), embutido no executável;
#   sem isto o app não abre num Windows sem o "Visual C++ Redistributable".
# - macOS: Universal 2 (x86_64 + arm64) e alvo 10.15 (std::filesystem nas
#   ferramentas exige 10.15; a fatia arm64 exige 11.0 de qualquer forma).
#   Precisa vir antes do project().
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
if(APPLE)
    set(CMAKE_OSX_DEPLOYMENT_TARGET "10.15" CACHE STRING "Minimum macOS version" FORCE)
    set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64" CACHE STRING "Build a Universal 2 binary" FORCE)
endif()

project(Batruc VERSION 0.1.3 LANGUAGES C CXX)

# Pilha de 8 MB no Windows, como o padrão do Linux. O Windows reserva só 1 MB
# por padrão, e a composição guarda estruturas grandes na pilha
# (FigureComposition ~180 KB; alguns testes passam de 3 MB). Sem isto, os
# testes caíam com SegFault na primeira rodada da CI (13/11/2026).
if(MSVC)
    add_link_options(/STACK:8388608)
endif()

# Linux: ligar só as bibliotecas realmente usadas. O GCC do Ubuntu já faz isso
# por padrão; o Clang não, e a checagem de std::atomic do JUCE deixava o
# binário pedindo libatomic.so.1 sem usar nenhum símbolo dela. No Fedora, que
# não traz essa biblioteca, o .tar.gz não abria (CI, 13/11/2026).
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    add_link_options("LINKER:--as-needed")
endif()

add_library(batruc_core STATIC
    src/core/CompositionCore.cpp
    src/core/BassComposition.cpp
    src/core/BassAudio.cpp
    src/core/GenerativeOperations.cpp
    src/core/EcologicalRelations.cpp
    src/core/GeometryCurves.cpp
    src/core/TriangleGeometry.cpp
    src/core/MidiRhythmImport.cpp
    src/core/FigureComposer.cpp
    src/core/SnareOrnaments.cpp
    src/core/BassLineComposer.cpp
    src/core/ScoreNotation.cpp
    # saida-ref 1.1 (16/11/2026): o master de entrega da gravação
    src/core/saida/DeliveryMaster.cpp
    src/core/saida/OutputStage.cpp
    src/core/saida/LoudnessMeter.cpp
    src/core/saida/TruePeakDetector.cpp
    src/core/saida/TakeAnalysis.cpp
)

target_include_directories(batruc_core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_features(batruc_core PUBLIC cxx_std_17)
set_target_properties(batruc_core PROPERTIES
    CXX_EXTENSIONS OFF
    POSITION_INDEPENDENT_CODE ON
)

if(MSVC)
    target_compile_options(batruc_core PRIVATE /W4 /permissive-)
else()
    target_compile_options(batruc_core PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Deterministic offline render for repeatable JUCE-core audio comparisons.
add_executable(batruc_offline_render src/tools/offline_render.cpp)
target_link_libraries(batruc_offline_render PRIVATE batruc_core)

# Bounded MIDI-to-stem reference render for inspecting musical role mapping.
add_executable(batruc_midi_reference_render src/tools/midi_reference_render.cpp)
target_link_libraries(batruc_midi_reference_render PRIVATE batruc_core)

include(CTest)
if(BUILD_TESTING)
    add_executable(batruc_output_safety_test tests/output_safety.cpp)
    target_link_libraries(batruc_output_safety_test PRIVATE batruc_core)
    add_test(NAME batruc_output_safety COMMAND batruc_output_safety_test)

    add_executable(batruc_figure_composer_test tests/figure_composer.cpp)
    target_link_libraries(batruc_figure_composer_test PRIVATE batruc_core)
    add_test(NAME batruc_figure_composer COMMAND batruc_figure_composer_test)

    add_executable(batruc_rhythm_mode_test tests/rhythm_mode.cpp)
    target_link_libraries(batruc_rhythm_mode_test PRIVATE batruc_core)
    add_test(NAME batruc_rhythm_mode COMMAND batruc_rhythm_mode_test)
    add_executable(batruc_generative_depth_test tests/generative_depth.cpp)
    target_link_libraries(batruc_generative_depth_test PRIVATE batruc_core)
    add_test(NAME batruc_generative_depth COMMAND batruc_generative_depth_test)

    add_executable(batruc_bass_sanitize_test tests/bass_sanitize.cpp)
    target_link_libraries(batruc_bass_sanitize_test PRIVATE batruc_core)
    add_test(NAME batruc_bass_sanitize COMMAND batruc_bass_sanitize_test)

    add_executable(batruc_bass_key_test tests/bass_key.cpp)
    target_link_libraries(batruc_bass_key_test PRIVATE batruc_core)
    add_test(NAME batruc_bass_key COMMAND batruc_bass_key_test)

    add_executable(batruc_formal_cycle_cells_test tests/formal_cycle_cells.cpp)
    target_link_libraries(batruc_formal_cycle_cells_test PRIVATE batruc_core)
    add_test(NAME batruc_formal_cycle_cells COMMAND batruc_formal_cycle_cells_test)

    add_executable(batruc_snare_ornaments_test tests/snare_ornaments.cpp)
    target_link_libraries(batruc_snare_ornaments_test PRIVATE batruc_core)
    add_test(NAME batruc_snare_ornaments COMMAND batruc_snare_ornaments_test)
    add_executable(batruc_bass_line_test tests/bass_line.cpp)
    target_link_libraries(batruc_bass_line_test PRIVATE batruc_core)
    add_test(NAME batruc_bass_line COMMAND batruc_bass_line_test)
    add_executable(batruc_percussion_key_test tests/percussion_key.cpp)
    target_link_libraries(batruc_percussion_key_test PRIVATE batruc_core)
    add_test(NAME batruc_percussion_key COMMAND batruc_percussion_key_test)
    add_executable(batruc_score_notation_test tests/score_notation.cpp)
    target_link_libraries(batruc_score_notation_test PRIVATE batruc_core)
    add_test(NAME batruc_score_notation COMMAND batruc_score_notation_test)

    add_executable(batruc_seeded_recipe_test
        tests/seeded_recipe.cpp
    )
    target_link_libraries(batruc_seeded_recipe_test PRIVATE batruc_core)
    add_test(NAME batruc_seeded_recipe
             COMMAND batruc_seeded_recipe_test)

    add_executable(batruc_variant_transport_test
        tests/variant_transport.cpp
    )
    target_link_libraries(batruc_variant_transport_test PRIVATE batruc_core)
    add_test(NAME batruc_variant_transport
             COMMAND batruc_variant_transport_test)

    add_executable(batruc_bass_composition_test
        tests/bass_composition.cpp
    )
    target_link_libraries(batruc_bass_composition_test PRIVATE batruc_core)
    add_test(NAME batruc_bass_composition
             COMMAND batruc_bass_composition_test)

    add_executable(batruc_bass_audio_response_test
        tests/bass_audio_response.cpp
    )
    target_link_libraries(batruc_bass_audio_response_test PRIVATE batruc_core)
    add_test(NAME batruc_bass_audio_response
             COMMAND batruc_bass_audio_response_test)

    add_executable(batruc_ecological_bass_composition_test
        tests/ecological_bass_composition.cpp
    )
    target_link_libraries(batruc_ecological_bass_composition_test PRIVATE batruc_core)
    add_test(NAME batruc_ecological_bass_composition
             COMMAND batruc_ecological_bass_composition_test)

    add_executable(batruc_seeded_bass_fixture
        tests/seeded_bass_fixture.cpp
    )
    target_link_libraries(batruc_seeded_bass_fixture PRIVATE batruc_core)
    add_executable(batruc_seeded_variation_fixture
        tests/seeded_variation_fixture.cpp
    )
    target_link_libraries(batruc_seeded_variation_fixture PRIVATE batruc_core)
    find_program(BATRUC_NODE_EXECUTABLE node)
    if(BATRUC_NODE_EXECUTABLE AND NOT WIN32)
        add_test(NAME batruc_seeded_bass_parity
            COMMAND ${CMAKE_COMMAND} -E env
                "BATRUC_NODE_EXECUTABLE=${BATRUC_NODE_EXECUTABLE}"
                /bin/sh
                ${CMAKE_CURRENT_SOURCE_DIR}/tools/test_seeded_bass_parity.sh
                $<TARGET_FILE:batruc_seeded_bass_fixture>
                ${CMAKE_CURRENT_BINARY_DIR}/seeded-bass-parity
                ${CMAKE_CURRENT_SOURCE_DIR})
        add_test(NAME batruc_seeded_variation_parity
            COMMAND ${CMAKE_COMMAND} -E env
                "BATRUC_NODE_EXECUTABLE=${BATRUC_NODE_EXECUTABLE}"
                /bin/sh
                ${CMAKE_CURRENT_SOURCE_DIR}/tools/test_seeded_variation_parity.sh
                $<TARGET_FILE:batruc_seeded_variation_fixture>
                ${CMAKE_CURRENT_BINARY_DIR}/seeded-variation-parity
                ${CMAKE_CURRENT_SOURCE_DIR})
        add_test(NAME batruc_seeded_voice_bass_parity
            COMMAND ${CMAKE_COMMAND} -E env
                "BATRUC_NODE_EXECUTABLE=${BATRUC_NODE_EXECUTABLE}"
                /bin/sh
                ${CMAKE_CURRENT_SOURCE_DIR}/tools/test_seeded_voice_bass_parity.sh
                $<TARGET_FILE:batruc_seeded_bass_fixture>
                ${CMAKE_CURRENT_BINARY_DIR}/seeded-voice-bass-parity
                ${CMAKE_CURRENT_SOURCE_DIR})
        add_test(NAME batruc_cyclic_bass_parity
            COMMAND ${CMAKE_COMMAND} -E env
                "BATRUC_NODE_EXECUTABLE=${BATRUC_NODE_EXECUTABLE}"
                /bin/sh
                ${CMAKE_CURRENT_SOURCE_DIR}/tools/test_cyclic_bass_parity.sh
                $<TARGET_FILE:batruc_seeded_bass_fixture>
                ${CMAKE_CURRENT_BINARY_DIR}/cyclic-bass-parity
                ${CMAKE_CURRENT_SOURCE_DIR})
    endif()

    add_executable(batruc_voice_inversion_test
        tests/voice_inversion.cpp
    )
    target_link_libraries(batruc_voice_inversion_test PRIVATE batruc_core)
    add_test(NAME batruc_voice_inversion
             COMMAND batruc_voice_inversion_test)

    add_executable(batruc_generative_operations_test
        tests/generative_operations.cpp
    )
    target_link_libraries(batruc_generative_operations_test PRIVATE batruc_core)
    add_test(NAME batruc_generative_operations
             COMMAND batruc_generative_operations_test)

    add_executable(batruc_triangle_geometry_test
        tests/triangle_geometry.cpp
    )
    target_link_libraries(batruc_triangle_geometry_test PRIVATE batruc_core)
    add_test(NAME batruc_triangle_geometry
             COMMAND batruc_triangle_geometry_test)

    add_executable(batruc_geometry_curves_test
        tests/geometry_curves.cpp
    )
    target_link_libraries(batruc_geometry_curves_test PRIVATE batruc_core)
    add_test(NAME batruc_geometry_curves
             COMMAND batruc_geometry_curves_test)

    add_executable(batruc_geometry_curve_fixture
        tests/geometry_curve_fixture.cpp
    )
    target_link_libraries(batruc_geometry_curve_fixture PRIVATE batruc_core)
    if(BATRUC_NODE_EXECUTABLE)
        add_test(NAME batruc_geometry_curve_parity
            COMMAND ${BATRUC_NODE_EXECUTABLE}
                ${CMAKE_CURRENT_SOURCE_DIR}/tests/geometry_curve_parity.cjs
                $<TARGET_FILE:batruc_geometry_curve_fixture>)
    endif()

    add_executable(batruc_ecological_relations_fixture
        tests/ecological_relations_fixture.cpp
    )
    target_link_libraries(batruc_ecological_relations_fixture PRIVATE batruc_core)
    if(BATRUC_NODE_EXECUTABLE)
        add_test(NAME batruc_ecological_relations_parity
            COMMAND ${BATRUC_NODE_EXECUTABLE}
                ${CMAKE_CURRENT_SOURCE_DIR}/tests/ecological_relations_parity.cjs
                $<TARGET_FILE:batruc_ecological_relations_fixture>)
    endif()

    add_executable(batruc_mixer_routing_test
        tests/mixer_routing.cpp
    )
    target_link_libraries(batruc_mixer_routing_test PRIVATE batruc_core)
    add_test(NAME batruc_mixer_routing
             COMMAND batruc_mixer_routing_test)

    add_executable(batruc_midi_rhythm_import_test
        tests/midi_rhythm_import.cpp
    )
    target_link_libraries(batruc_midi_rhythm_import_test PRIVATE batruc_core)
    add_test(NAME batruc_midi_rhythm_import
             COMMAND batruc_midi_rhythm_import_test)
endif()

option(BATRUC_BUILD_JUCE_APP "Build the native JUCE standalone" OFF)
set(BATRUC_JUCE_SOURCE_DIR "" CACHE PATH
    "Path to the exact JUCE 9.0.3 source tree")

if(BATRUC_BUILD_JUCE_APP)
    if(NOT BATRUC_JUCE_SOURCE_DIR
       OR NOT EXISTS "${BATRUC_JUCE_SOURCE_DIR}/CMakeLists.txt")
        message(FATAL_ERROR
            "Set BATRUC_JUCE_SOURCE_DIR to the JUCE 9.0.3 source tree")
    endif()

    file(STRINGS "${BATRUC_JUCE_SOURCE_DIR}/CMakeLists.txt"
         _batruc_juce_project_line
         REGEX "^project\\(JUCE VERSION [0-9]+\\.[0-9]+\\.[0-9]+ LANGUAGES C CXX\\)")
    if(NOT _batruc_juce_project_line STREQUAL
       "project(JUCE VERSION 9.0.3 LANGUAGES C CXX)")
        message(FATAL_ERROR
            "Batruc requires JUCE 9.0.3 exactly; found: ${_batruc_juce_project_line}")
    endif()

    add_subdirectory("${BATRUC_JUCE_SOURCE_DIR}"
                     "${CMAKE_BINARY_DIR}/JUCE" EXCLUDE_FROM_ALL)

    # ícone embutido no executável (Windows/macOS) e no .desktop (Linux):
    # o B original, derivado em prototype/assets/brand/icons/
    juce_add_gui_app(batruc_app
        PRODUCT_NAME "Batruc"
        COMPANY_NAME "RASGO"
        VERSION "${PROJECT_VERSION}"
        ICON_BIG "${CMAKE_CURRENT_SOURCE_DIR}/prototype/assets/brand/icons/icon-512.png"
        ICON_SMALL "${CMAKE_CURRENT_SOURCE_DIR}/prototype/assets/brand/icons/favicon-32.png")
    target_sources(batruc_app PRIVATE src/app/Main.cpp)
    juce_add_binary_data(batruc_brand_data SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/prototype/assets/brand/active/batruc-wordmark-original-crop-v01.png"
    )
    target_compile_definitions(batruc_app PRIVATE
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_DISPLAY_SPLASH_SCREEN=0
        JUCE_REPORT_APP_USAGE=0
        JUCE_APPLICATION_NAME_STRING="Batruc"
        JUCE_APPLICATION_VERSION_STRING="${PROJECT_VERSION}"
    )
    if(MSVC)
        # /bigobj: Main.cpp passa de 23 mil linhas (o Rasgo Modular bateu no
        # erro C1128 do MSVC com a unidade principal grande)
        target_compile_options(batruc_app PRIVATE /utf-8 /bigobj)
    else()
        target_compile_options(batruc_app PRIVATE
            -finput-charset=UTF-8
            -fexec-charset=UTF-8
        )
    endif()
    target_link_libraries(batruc_app PRIVATE
        batruc_core
        batruc_brand_data
        juce::juce_audio_utils
        juce::juce_cryptography
        juce::juce_gui_extra
        juce::juce_osc   # receptor do Maestruc (MaestrucLink.h), 14/11/2026
        juce::juce_recommended_config_flags
        juce::juce_recommended_warning_flags
    )
endif()

# ---- instalação e empacotamento (CPack), 13/11/2026 ----------------------
# Mesmo desenho do Rasgo Modular: nome de arquivo determinístico com a
# plataforma; .deb no Linux, instalador NSIS no Windows (pasta e chave sem a
# versão, atalhos apontando para o .exe na raiz), .dmg no macOS.
if(BATRUC_BUILD_JUCE_APP AND TARGET batruc_app)
    if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        install(PROGRAMS "$<TARGET_FILE:batruc_app>" DESTINATION bin RENAME batruc)
        install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/packaging/linux/batruc.desktop"
            DESTINATION share/applications)
        install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/prototype/assets/brand/icons/favicon.svg"
            DESTINATION share/icons/hicolor/scalable/apps RENAME batruc.svg)
        install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/prototype/assets/brand/icons/icon-512.png"
            DESTINATION share/icons/hicolor/512x512/apps RENAME batruc.png)
    elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
        install(TARGETS batruc_app RUNTIME DESTINATION .)
    elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
        install(TARGETS batruc_app BUNDLE DESTINATION .)
    endif()

    # Catálogo Ritmos / MIDI: o Groove MIDI Dataset (Google LLC, CC BY 4.0)
    # vai junto quando está presente em midis/groove (baixado e conferido por
    # packaging/obter-gmd.sh; não é versionado). Decisão do autor, 13/11/2026.
    option(BATRUC_BUNDLE_GMD "Install the Groove MIDI Dataset with the app" ON)
    if(BATRUC_BUNDLE_GMD AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/midis/groove/info.csv")
        if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
            set(_batruc_gmd_destination "share/batruc/midis")
        elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
            set(_batruc_gmd_destination "Batruc.app/Contents/Resources/midis")
        else()
            set(_batruc_gmd_destination "midis")
        endif()
        install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/midis/groove"
            DESTINATION "${_batruc_gmd_destination}"
            FILES_MATCHING PATTERN "*.mid" PATTERN "*.midi" PATTERN "info.csv"
                           PATTERN "LICENSE" PATTERN "README")
        message(STATUS "batruc: o pacote leva o Groove MIDI Dataset (CC BY 4.0)")
    else()
        message(STATUS "batruc: pacote SEM o Groove MIDI Dataset (rode packaging/obter-gmd.sh)")
    endif()

    set(CPACK_PACKAGE_NAME "batruc")
    set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
    set(CPACK_PACKAGE_VENDOR "RASGO")
    set(CPACK_PACKAGE_CONTACT "Lúcio Araújo <rasgo.instruments@gmail.com>")
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY
        "Batruc — instrumento de composição rítmica por matemática e física")
    set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
    if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
        list(LENGTH CMAKE_OSX_ARCHITECTURES _batruc_n_arch)
        if(_batruc_n_arch GREATER 1)
            set(_batruc_plat "macos-universal")
        else()
            set(_batruc_plat "macos-${CMAKE_SYSTEM_PROCESSOR}")
        endif()
    elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
        set(_batruc_plat "windows-x64")
    else()
        string(TOLOWER "${CMAKE_SYSTEM_NAME}" _batruc_os)
        set(_batruc_plat "${_batruc_os}-${CMAKE_SYSTEM_PROCESSOR}")
    endif()
    set(CPACK_PACKAGE_FILE_NAME "batruc-${PROJECT_VERSION}-${_batruc_plat}")

    if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        set(CPACK_PACKAGING_INSTALL_PREFIX "/usr")
        set(CPACK_GENERATOR "DEB")
        set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Lúcio Araújo <rasgo.instruments@gmail.com>")
        set(CPACK_DEBIAN_PACKAGE_SECTION "sound")
        set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
    elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
        set(CPACK_GENERATOR "NSIS")
        set(CPACK_NSIS_PACKAGE_NAME "Batruc")
        set(CPACK_NSIS_DISPLAY_NAME "Batruc")
        set(CPACK_NSIS_MODIFY_PATH OFF)
        set(CPACK_PACKAGE_INSTALL_DIRECTORY "Batruc")
        set(CPACK_PACKAGE_INSTALL_REGISTRY_KEY "Batruc")
        set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)
        set(CPACK_PACKAGE_EXECUTABLES "Batruc" "Batruc")
        set(CPACK_NSIS_EXECUTABLES_DIRECTORY ".")
        set(CPACK_CREATE_DESKTOP_LINKS "Batruc")
        set(CPACK_NSIS_INSTALLED_ICON_NAME "Batruc.exe")
        set(CPACK_NSIS_MUI_FINISHPAGE_RUN "Batruc.exe")
        set(CPACK_NSIS_MUI_ICON "${CMAKE_CURRENT_SOURCE_DIR}/prototype/assets/brand/icons/favicon.ico")
        set(CPACK_NSIS_MUI_UNIICON "${CMAKE_CURRENT_SOURCE_DIR}/prototype/assets/brand/icons/favicon.ico")
    elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
        set(CPACK_GENERATOR "DragNDrop")
        set(CPACK_DMG_VOLUME_NAME "Batruc")
    endif()
    include(CPack)
endif()
