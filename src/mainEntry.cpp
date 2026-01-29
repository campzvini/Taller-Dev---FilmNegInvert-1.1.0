/*
══════════════════════════════════════════════════════════════════════════════
 FILM NEG INVERT 1.0 - MAINENTRY.CPP
 Entry point OFX para registro do plugin Film Neg Invert usando Support Library C++
 C++ Source
══════════════════════════════════════════════════════════════════════════════
Taller Dev - 2026
VAI CORINTHIANS!!
══════════════════════════════════════════════════════════════════════════════
*/

// ============================================
// 1. INCLUDES # Bibliotecas necessárias
// ============================================
#include "FilmNegInvertPluginFactory.h"
#include "ofxsImageEffect.h"

// ============================================
// 2. FACTORY INSTANCE # Instância da factory
// ============================================
static FilmNegInvert::FilmNegInvertPluginFactory gFilmNegInvertPluginFactory;

// ============================================
// 3. OFX ENTRY POINT # Entry point OFX
// ============================================
// As funções OfxGetNumberOfPlugins e OfxGetPlugin são geradas automaticamente
// pela Support Library em ofxsImageEffect.cpp

// Registra a factory com a Support Library
namespace OFX {
    void Plugin::getPluginIDs(OFX::PluginFactoryArray &ids) {
        ids.push_back(&gFilmNegInvertPluginFactory);
    }
}
