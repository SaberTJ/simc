// ==========================================================================
// Dedmonwakeen's Raid DPS/TPS Simulator.
// Send questions to natehieter@gmail.com
// ==========================================================================
#ifndef SIMULATIONCRAFT_H
#define SIMULATIONCRAFT_H

// Platform Initialization ==================================================

#if defined( _MSC_VER ) || defined( __MINGW__ ) || defined( __MINGW32__ ) || defined( _WINDOWS ) || defined( WIN32 )
#  define WIN32_LEAN_AND_MEAN
#  define VC_EXTRALEAN
#  ifndef _CRT_SECURE_NO_WARNINGS
#    define _CRT_SECURE_NO_WARNINGS
#  endif
#  define DIRECTORY_DELIMITER "\\"
#  ifndef UNICODE
#    define UNICODE
#  endif
#else
#  define DIRECTORY_DELIMITER "/"
#  define SC_SIGACTION
#endif

#if defined( _MSC_VER )
#  include "../vs/stdint.h"
#else
#  include <stdint.h>
#endif

#include <algorithm>
#include <cassert>
#include <cctype>
#include <cfloat>
#include <cmath>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <functional>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <list>
#include <map>
#include <memory>
#include <numeric>
#include <queue>
#include <sstream>
#include <stack>
#include <string>
#include <typeinfo>
#include <vector>

#if _MSC_VER || __cplusplus >= 201103L || defined(__GXX_EXPERIMENTAL_CXX0X__)
// Use C++11
#include <array>
#include <type_traits>
#include <unordered_map>
#if ( _MSC_VER && _MSC_VER < 1600 )
namespace std {using namespace tr1; }
#endif
#else
// Use TR1
#include <tr1/array>
#include <tr1/functional>
#include <tr1/type_traits>
#include <tr1/unordered_map>
namespace std {using namespace tr1; }
#endif

#include "dbc/data_enums.hh"
#include "dbc/data_definitions.hh"
#include "utf8.h"

#if __BSD_VISIBLE
#  include <netinet/in.h>
#  if !defined(CLOCKS_PER_SEC)
#    define CLOCKS_PER_SEC 1000000
#  endif
#endif

#if _MSC_VER >= 1600 || __cplusplus >= 201103L || defined(__GXX_EXPERIMENTAL_CXX0X__)
#define smart_ptr unique_ptr
#else
#define smart_ptr auto_ptr
#define static_assert( condition, message )
#endif

// GCC (and probably the C++ standard in general) doesn't like offsetof on non-POD types
#ifdef _MSC_VER
#define nonpod_offsetof(t, m) offsetof(t, m)
#else
#define nonpod_offsetof(t, m) ((size_t) ( (volatile char *)&((volatile t *)(size_t)0x10000)->m - (volatile char *)(size_t)0x10000 ))
#endif

#if defined(__GNUC__)
#  define likely(x)         __builtin_expect((x),1)
#  define unlikely(x)       __builtin_expect((x),0)
#else
#  define likely(x)        (x)
#  define unlikely(x)      (x)
#  define __attribute__(x)
#endif

#define SC_PACKED_STRUCT      __attribute__((packed))
#define PRINTF_ATTRIBUTE(a,b) __attribute__((format(printf,a,b)))

#define SC_MAJOR_VERSION "500"
#define SC_MINOR_VERSION "1"
#define SC_USE_PTR ( 0 )
#define SC_BETA ( 1 )
#define SC_EPSILON ( 0.000001 )
#ifndef M_PI
#define M_PI ( 3.14159265358979323846 )
#endif

// Enabled classes
#define SC_DEATH_KNIGHT 0
#define SC_DRUID        0
#define SC_HUNTER       0
#define SC_MAGE         1
#define SC_MONK         0
#define SC_PALADIN      0
#define SC_PRIEST       1
#define SC_ROGUE        0
#define SC_SHAMAN       0
#define SC_WARLOCK      0
#define SC_WARRIOR      0

#define BLOCKED_CLASS( sim, name_str, pt, r_type ) { \
  ( void ) name_str; ( void ) r_type; \
  std::string class_string = util_t::player_type_string( pt ); \
  if ( ! class_string.empty() ) \
    class_string[ 0 ] = std::toupper( class_string[ 0 ] ); \
  sim -> errorf( "%s", std::string( "\n" + class_string + " module is currently not available.\n" ).c_str() ); \
  return 0; \
}

#if SC_DEATH_KNIGHT == 1
#define SC_CREATE_DEATH_KNIGHT( sim, name_str, r_type ) return new death_knight_t( sim, name_str, r_type )
#else
#define SC_CREATE_DEATH_KNIGHT( sim, name_str, r_type ) BLOCKED_CLASS( sim, name_str, DEATH_KNIGHT, r_type )
#endif

#if SC_DRUID == 1
#define SC_CREATE_DRUID( sim, name_str, r_type ) return new druid_t( sim, name_str, r_type )
#else
#define SC_CREATE_DRUID( sim, name_str, r_type ) BLOCKED_CLASS( sim, name_str, DRUID, r_type )
#endif

#if SC_HUNTER == 1
#define SC_CREATE_HUNTER( sim, name_str, r_type ) return new hunter_t( sim, name_str, r_type )
#else
#define SC_CREATE_HUNTER( sim, name_str, r_type ) BLOCKED_CLASS( sim, name_str, HUNTER, r_type )
#endif

#if SC_MAGE == 1
#define SC_CREATE_MAGE( sim, name_str, r_type ) return new mage_t( sim, name_str, r_type )
#else
#define SC_CREATE_MAGE( sim, name_str, r_type ) BLOCKED_CLASS( sim, name_str, MAGE, r_type )
#endif

#if SC_MONK == 1
#define SC_CREATE_MONK( sim, name_str, r_type ) return new monk_t( sim, name_str, r_type )
#else
#define SC_CREATE_MONK( sim, name_str, r_type ) BLOCKED_CLASS( sim, name_str, MONK, r_type )
#endif

#if SC_PALADIN == 1
#define SC_CREATE_PALADIN( sim, name_str, r_type ) return new paladin_t( sim, name_str, r_type )
#else
#define SC_CREATE_PALADIN( sim, name_str, r_type ) BLOCKED_CLASS( sim, name_str, PALADIN, r_type )
#endif

#if SC_PRIEST == 1
#define SC_CREATE_PRIEST( sim, name_str, r_type ) return new priest_t( sim, name_str, r_type )
#else
#define SC_CREATE_PRIEST( sim, name_str, r_type ) BLOCKED_CLASS( sim, name_str, PRIEST, r_type )
#endif

#if SC_ROGUE == 1
#define SC_CREATE_ROGUE( sim, name_str, r_type ) return new rogue_t( sim, name_str, r_type )
#else
#define SC_CREATE_ROGUE( sim, name_str, r_type ) BLOCKED_CLASS( sim, name_str, ROGUE, r_type )
#endif

#if SC_SHAMAN == 1
#define SC_CREATE_SHAMAN( sim, name_str, r_type ) return new shaman_t( sim, name_str, r_type )
#else
#define SC_CREATE_SHAMAN( sim, name_str, r_type ) BLOCKED_CLASS( sim, name_str, SHAMAN, r_type )
#endif

#if SC_WARLOCK == 1
#define SC_CREATE_WARLOCK( sim, name_str, r_type ) return new warlock_t( sim, name_str, r_type )
#else
#define SC_CREATE_WARLOCK( sim, name_str, r_type ) BLOCKED_CLASS( sim, name_str, WARLOCK, r_type )
#endif

#if SC_WARRIOR == 1
#define SC_CREATE_WARRIOR( sim, name_str, r_type ) return new warrior_t( sim, name_str, r_type )
#else
#define SC_CREATE_WARRIOR( sim, name_str, r_type ) BLOCKED_CLASS( sim, name_str, WARRIOR, r_type )
#endif

#define MAX_PLAYERS_PER_CHART 20

// TODO: Integer time is only partially working.
#define SC_USE_INTEGER_TIME

// Forward Declarations =====================================================

struct action_callback_t;
struct action_priority_list_t;
struct action_state_t;
struct action_t;
struct alias_t;
struct attack_t;
struct benefit_t;
struct buff_t;
struct buff_creator_t;
struct buff_uptime_t;
struct callback_t;
struct cooldown_t;
struct cost_reduction_buff_t;
class  dbc_t;
struct death_knight_t;
struct debuff_t;
struct dot_t;
struct druid_t;
struct effect_t;
struct enchant_t;
struct enemy_t;
struct event_t;
class  expr_t;
struct gain_t;
struct heal_t;
struct hunter_t;
struct item_t;
struct js_node_t;
struct mage_t;
struct monk_t;
struct option_t;
struct paladin_t;
struct pet_t;
struct player_t;
struct plot_t;
struct priest_t;
struct proc_t;
struct raid_event_t;
struct rating_t;
struct reforge_plot_data_t;
struct reforge_plot_t;
struct report_t;
struct rng_t;
struct rogue_t;
struct sample_data_t;
struct scaling_t;
struct shaman_t;
struct sim_t;
struct spell_data_t;
struct spell_id_t;
struct spell_t;
struct spelleffect_data_t;
struct stats_t;
struct stat_buff_t;
struct unique_gear_t;
struct uptime_t;
struct warlock_t;
struct warrior_t;
struct weapon_t;
struct xml_node_t;

struct targetdata_t;
struct death_knight_targetdata_t;
struct druid_targetdata_t;
struct hunter_targetdata_t;
struct mage_targetdata_t;
struct monk_targetdata_t;
struct paladin_targetdata_t;
struct priest_targetdata_t;
struct rogue_targetdata_t;
struct shaman_targetdata_t;
struct warlock_targetdata_t;
struct warrior_targetdata_t;

void register_death_knight_targetdata( sim_t* sim );
void register_druid_targetdata( sim_t* sim );
void register_hunter_targetdata( sim_t* sim );
void register_mage_targetdata( sim_t* sim );
void register_monk_targetdata( sim_t* sim );
void register_paladin_targetdata( sim_t* sim );
void register_priest_targetdata( sim_t* sim );
void register_rogue_targetdata( sim_t* sim );
void register_shaman_targetdata( sim_t* sim );
void register_warlock_targetdata( sim_t* sim );
void register_warrior_targetdata( sim_t* sim );

#define DATA_DOT 0
#define DATA_AURA 1
#define DATA_COUNT 2

struct actor_pair_t
{
  player_t* target;
  player_t* source;

  actor_pair_t( player_t* target, player_t* source )
    : target( target ), source( source )
  {}

  actor_pair_t( player_t* p = 0 )
    : target( p ), source( p )
  {}

  actor_pair_t( targetdata_t* td );
};

// Type traits and metaprogramming tools ====================================

template <bool Condition,typename T>
struct enable_if { typedef T type; };
template <typename T>
struct enable_if<false,T> {};

template <typename T>
struct iterator_type
{ typedef typename T::iterator type; };

template <typename T>
struct iterator_type<const T>
{ typedef typename T::const_iterator type; };

// iterable enumeration templates ===========================================

/*
 * Enumeration types in C++ implicitly convert to int but not from int (e.g.,
 * "for ( attribute_type_e i = ATTR_STRENGTH; i < ATTR_MAX; ++i )" won't compile).
 * This is why so much of the code uses int when it really means an enum type.
 * Providing the kind of operations we want to use for enums lets us tighten up
 * our use of the type system and avoid accidentally passing some other thing
 * that converts to int when we really mean an enumeration type.
 *
 * The template functions tell the compiler it can perform prefix and postfix
 * operators ++ and -- on any type by converting it to int and back. The magic
 * with std::enable_if<> restricts those operations to types T for which the
 * type trait "is_iterable_enum<T>" is true. This trait gives us a way to
 * selectively control the functionality for a specific type T by specializing
 * is_iterable_enum<T> as std::true_type or std::false_type.
 */

// All enumerations are iterable by default.
template <typename T>
struct is_iterable_enum : public std::is_enum<T> {};

template <typename T>
inline typename enable_if<is_iterable_enum<T>::value,T&>::type
operator -- ( T& s )
{ return s = static_cast<T>( s - 1 ); }

template <typename T>
inline typename enable_if<is_iterable_enum<T>::value,T>::type
operator -- ( T& s, int )
{
  T tmp = s;
  --s;
  return tmp;
}

template <typename T>
inline typename enable_if<is_iterable_enum<T>::value,T&>::type
operator ++ ( T& s )
{ return s = static_cast<T>( s + 1 ); }

template <typename T>
inline typename enable_if<is_iterable_enum<T>::value,T>::type
operator ++ ( T& s, int )
{
  T tmp = s;
  ++s;
  return tmp;
}

// Enumerations =============================================================
// annex _e to enumerations

enum race_type_e
{
  RACE_NONE=0,
  // Target Races
  RACE_BEAST, RACE_DRAGONKIN, RACE_GIANT, RACE_HUMANOID, RACE_DEMON, RACE_ELEMENTAL,
  // Player Races
  RACE_NIGHT_ELF, RACE_HUMAN, RACE_GNOME, RACE_DWARF, RACE_DRAENEI, RACE_WORGEN,
  RACE_ORC, RACE_TROLL, RACE_UNDEAD, RACE_BLOOD_ELF, RACE_TAUREN, RACE_GOBLIN,
  RACE_PANDAREN, RACE_PANDAREN_ALLIANCE, RACE_PANDAREN_HORDE,
  RACE_MAX
};

enum player_type_e
{
  PLAYER_SPECIAL_SCALE=-1,
  PLAYER_NONE=0,
  DEATH_KNIGHT, DRUID, HUNTER, MAGE, MONK, PALADIN, PRIEST, ROGUE, SHAMAN, WARLOCK, WARRIOR,
  PLAYER_PET, PLAYER_GUARDIAN,
  ENEMY, ENEMY_ADD,
  PLAYER_MAX
};

enum pet_type_e
{
  PET_NONE=0,

  // Ferocity
  PET_CARRION_BIRD,
  PET_CAT,
  PET_CORE_HOUND,
  PET_DEVILSAUR,
  PET_DOG,
  PET_FOX,
  PET_HYENA,
  PET_MOTH,
  PET_RAPTOR,
  PET_SPIRIT_BEAST,
  PET_TALLSTRIDER,
  PET_WASP,
  PET_WOLF,

  PET_FEROCITY_TYPE,

  // Tenacity
  PET_BEAR,
  PET_BEETLE,
  PET_BOAR,
  PET_CRAB,
  PET_CROCOLISK,
  PET_GORILLA,
  PET_RHINO,
  PET_SCORPID,
  PET_SHALE_SPIDER,
  PET_TURTLE,
  PET_WARP_STALKER,
  PET_WORM,

  PET_TENACITY_TYPE,

  // Cunning
  PET_BAT,
  PET_BIRD_OF_PREY,
  PET_CHIMERA,
  PET_DRAGONHAWK,
  PET_MONKEY,
  PET_NETHER_RAY,
  PET_RAVAGER,
  PET_SERPENT,
  PET_SILITHID,
  PET_SPIDER,
  PET_SPOREBAT,
  PET_WIND_SERPENT,

  PET_CUNNING_TYPE,

  PET_HUNTER,

  PET_FELGUARD,
  PET_FELHUNTER,
  PET_IMP,
  PET_VOIDWALKER,
  PET_SUCCUBUS,
  PET_INFERNAL,
  PET_DOOMGUARD,
  PET_EBON_IMP,
  PET_WARLOCK,

  PET_GHOUL,
  PET_BLOODWORMS,
  PET_DANCING_RUNE_WEAPON,
  PET_DEATH_KNIGHT,

  PET_TREANTS,
  PET_DRUID,

  PET_WATER_ELEMENTAL,
  PET_MAGE,

  PET_SHADOWFIEND,
  PET_PRIEST,

  PET_SPIRIT_WOLF,
  PET_FIRE_ELEMENTAL,
  PET_EARTH_ELEMENTAL,
  PET_SHAMAN,

  PET_ENEMY,

  PET_MAX
};

enum dmg_type_e { DMG_DIRECT=0, DMG_OVER_TIME=1, HEAL_DIRECT, HEAL_OVER_TIME, ABSORB };

enum stats_type_e { STATS_DMG, STATS_HEAL, STATS_ABSORB };

enum dot_behaviour_type_e { DOT_CLIP, DOT_REFRESH };

enum attribute_type_e { ATTRIBUTE_NONE=0, ATTR_STRENGTH, ATTR_AGILITY, ATTR_STAMINA, ATTR_INTELLECT, ATTR_SPIRIT, ATTRIBUTE_MAX };

enum base_stat_type_e_e { BASE_STAT_STRENGTH=0, BASE_STAT_AGILITY, BASE_STAT_STAMINA, BASE_STAT_INTELLECT, BASE_STAT_SPIRIT,
                          BASE_STAT_HEALTH, BASE_STAT_MANA,
                          BASE_STAT_MELEE_CRIT_PER_AGI, BASE_STAT_SPELL_CRIT_PER_INT,
                          BASE_STAT_DODGE_PER_AGI,
                          BASE_STAT_MELEE_CRIT, BASE_STAT_SPELL_CRIT, BASE_STAT_MP5, BASE_STAT_SPI_REGEN, BASE_STAT_MAX
                        };

enum resource_type_e
{
  RESOURCE_NONE = 0,
  RESOURCE_HEALTH,
  RESOURCE_MANA,
  RESOURCE_RAGE,
  RESOURCE_FOCUS,
  RESOURCE_ENERGY,
  RESOURCE_MONK_ENERGY,
  RESOURCE_RUNIC_POWER,
  RESOURCE_SOUL_SHARD,
  /* Unknown_1, */
  RESOURCE_HOLY_POWER,
  /* Unknown_2, */
  /* Unknown_3 */
  RESOURCE_CHI,
  RESOURCE_SHADOW_ORB,
  RESOURCE_BURNING_EMBER,
  RESOURCE_DEMONIC_FURY,
  /* Dummy resources for reporting */
  RESOURCE_RUNE,
  RESOURCE_RUNE_BLOOD,
  RESOURCE_RUNE_UNHOLY,
  RESOURCE_RUNE_FROST,
  RESOURCE_MAX
};

enum result_type_e
{
  RESULT_UNKNOWN=-1,
  RESULT_NONE=0,
  RESULT_MISS,  RESULT_DODGE, RESULT_PARRY,
  RESULT_BLOCK, RESULT_CRIT_BLOCK, RESULT_GLANCE, RESULT_CRIT, RESULT_HIT,
  RESULT_MAX
};

#define RESULT_HIT_MASK   ( (1<<RESULT_GLANCE) | (1<<RESULT_BLOCK) | (1<<RESULT_CRIT_BLOCK) | (1<<RESULT_CRIT) | (1<<RESULT_HIT) )
#define RESULT_CRIT_MASK  ( (1<<RESULT_CRIT) )
#define RESULT_MISS_MASK  ( (1<<RESULT_MISS) )
#define RESULT_BLOCK_MASK ( (1<<RESULT_BLOCK) )
#define RESULT_DODGE_MASK ( (1<<RESULT_DODGE) )
#define RESULT_PARRY_MASK ( (1<<RESULT_PARRY) )
#define RESULT_NONE_MASK  ( (1<<RESULT_NONE) )
#define RESULT_ALL_MASK  -1

enum proc_type_e
{
  PROC_NONE=0,
  PROC_DAMAGE,
  PROC_HEAL,
  PROC_TICK_DAMAGE,
  PROC_DIRECT_DAMAGE,
  PROC_DIRECT_HEAL,
  PROC_TICK_HEAL,
  PROC_ATTACK,
  PROC_SPELL,
  PROC_TICK,
  PROC_SPELL_AND_TICK,
  PROC_HEAL_SPELL,
  PROC_HARMFUL_SPELL,
  PROC_DAMAGE_HEAL,
  PROC_MAX
};

enum action_type_e { ACTION_USE=0, ACTION_SPELL, ACTION_ATTACK, ACTION_HEAL, ACTION_ABSORB, ACTION_SEQUENCE, ACTION_OTHER, ACTION_MAX };

enum school_type_e
{
  SCHOOL_NONE=0,
  SCHOOL_ARCANE,      SCHOOL_FIRE,        SCHOOL_FROST,       SCHOOL_HOLY,        SCHOOL_NATURE,
  SCHOOL_SHADOW,      SCHOOL_PHYSICAL,    SCHOOL_MAX_PRIMARY, SCHOOL_FROSTFIRE,
  SCHOOL_HOLYSTRIKE,  SCHOOL_FLAMESTRIKE, SCHOOL_HOLYFIRE,    SCHOOL_STORMSTRIKE, SCHOOL_HOLYSTORM,
  SCHOOL_FIRESTORM,   SCHOOL_FROSTSTRIKE, SCHOOL_HOLYFROST,   SCHOOL_FROSTSTORM,  SCHOOL_SHADOWSTRIKE,
  SCHOOL_SHADOWLIGHT, SCHOOL_SHADOWFLAME, SCHOOL_SHADOWSTORM, SCHOOL_SHADOWFROST, SCHOOL_SPELLSTRIKE,
  SCHOOL_DIVINE,      SCHOOL_SPELLFIRE,   SCHOOL_SPELLSTORM,  SCHOOL_SPELLFROST,  SCHOOL_SPELLSHADOW,
  SCHOOL_ELEMENTAL,   SCHOOL_CHROMATIC,   SCHOOL_MAGIC,       SCHOOL_CHAOS,       SCHOOL_BLEED,
  SCHOOL_DRAIN,
  SCHOOL_MAX
};

const int64_t SCHOOL_ATTACK_MASK = ( ( int64_t( 1 ) << SCHOOL_BLEED )        | ( int64_t( 1 ) << SCHOOL_PHYSICAL )     |
                                     ( int64_t( 1 ) << SCHOOL_HOLYSTRIKE )   | ( int64_t( 1 ) << SCHOOL_FLAMESTRIKE )  |
                                     ( int64_t( 1 ) << SCHOOL_STORMSTRIKE )  | ( int64_t( 1 ) << SCHOOL_FROSTSTRIKE )  |
                                     ( int64_t( 1 ) << SCHOOL_SHADOWSTRIKE ) | ( int64_t( 1 ) << SCHOOL_SPELLSTRIKE )  );
                                      // SCHOOL_CHAOS should probably be added here too.

const int64_t SCHOOL_SPELL_MASK  ( ( int64_t( 1 ) << SCHOOL_ARCANE )         | ( int64_t( 1 ) << SCHOOL_CHAOS )        |
                                   ( int64_t( 1 ) << SCHOOL_FIRE )           | ( int64_t( 1 ) << SCHOOL_FROST )        |
                                   ( int64_t( 1 ) << SCHOOL_FROSTFIRE )      | ( int64_t( 1 ) << SCHOOL_HOLY )         |
                                   ( int64_t( 1 ) << SCHOOL_NATURE )         | ( int64_t( 1 ) << SCHOOL_SHADOW )       |
                                   ( int64_t( 1 ) << SCHOOL_HOLYSTRIKE )     | ( int64_t( 1 ) << SCHOOL_FLAMESTRIKE )  |
                                   ( int64_t( 1 ) << SCHOOL_HOLYFIRE )       | ( int64_t( 1 ) << SCHOOL_STORMSTRIKE )  |
                                   ( int64_t( 1 ) << SCHOOL_HOLYSTORM )      | ( int64_t( 1 ) << SCHOOL_FIRESTORM )    |
                                   ( int64_t( 1 ) << SCHOOL_FROSTSTRIKE )    | ( int64_t( 1 ) << SCHOOL_HOLYFROST )    |
                                   ( int64_t( 1 ) << SCHOOL_FROSTSTORM )     | ( int64_t( 1 ) << SCHOOL_SHADOWSTRIKE ) |
                                   ( int64_t( 1 ) << SCHOOL_SHADOWLIGHT )    | ( int64_t( 1 ) << SCHOOL_SHADOWFLAME )  |
                                   ( int64_t( 1 ) << SCHOOL_SHADOWSTORM )    | ( int64_t( 1 ) << SCHOOL_SHADOWFROST )  |
                                   ( int64_t( 1 ) << SCHOOL_SPELLSTRIKE )    | ( int64_t( 1 ) << SCHOOL_DIVINE )       |
                                   ( int64_t( 1 ) << SCHOOL_SPELLFIRE )      | ( int64_t( 1 ) << SCHOOL_SPELLSTORM )   |
                                   ( int64_t( 1 ) << SCHOOL_SPELLFROST )     | ( int64_t( 1 ) << SCHOOL_SPELLSHADOW )  |
                                   ( int64_t( 1 ) << SCHOOL_ELEMENTAL )      | ( int64_t( 1 ) << SCHOOL_CHROMATIC )    |
                                   ( int64_t( 1 ) << SCHOOL_MAGIC ) );

const int64_t SCHOOL_MAGIC_MASK  ( ( int64_t( 1 ) << SCHOOL_ARCANE )         |
                                   ( int64_t( 1 ) << SCHOOL_FIRE )           | ( int64_t( 1 ) << SCHOOL_FROST )        |
                                   ( int64_t( 1 ) << SCHOOL_FROSTFIRE )      | ( int64_t( 1 ) << SCHOOL_HOLY )         |
                                   ( int64_t( 1 ) << SCHOOL_NATURE )         | ( int64_t( 1 ) << SCHOOL_SHADOW ) );
#define SCHOOL_ALL_MASK    ( int64_t( -1 ) )

#include "dbc/specialization.hpp"

enum weapon_type_e
{
  WEAPON_NONE=0,
  WEAPON_DAGGER,                                                                                   WEAPON_SMALL,
  WEAPON_BEAST,    WEAPON_SWORD,    WEAPON_MACE,     WEAPON_AXE,    WEAPON_FIST,                   WEAPON_1H,
  WEAPON_BEAST_2H, WEAPON_SWORD_2H, WEAPON_MACE_2H,  WEAPON_AXE_2H, WEAPON_STAFF,  WEAPON_POLEARM, WEAPON_2H,
  WEAPON_BOW,      WEAPON_CROSSBOW, WEAPON_GUN,      WEAPON_WAND,   WEAPON_THROWN,                 WEAPON_RANGED,
  WEAPON_MAX
};

enum glyph_type_e
{
  GLYPH_MAJOR=0,
  GLYPH_MINOR,
  GLYPH_PRIME,
  GLYPH_MAX
};

enum slot_type_e   // these enum values match armory settings
{
  SLOT_INVALID   = -1,
  SLOT_HEAD      = 0,
  SLOT_NECK      = 1,
  SLOT_SHOULDERS = 2,
  SLOT_SHIRT     = 3,
  SLOT_CHEST     = 4,
  SLOT_WAIST     = 5,
  SLOT_LEGS      = 6,
  SLOT_FEET      = 7,
  SLOT_WRISTS    = 8,
  SLOT_HANDS     = 9,
  SLOT_FINGER_1  = 10,
  SLOT_FINGER_2  = 11,
  SLOT_TRINKET_1 = 12,
  SLOT_TRINKET_2 = 13,
  SLOT_BACK      = 14,
  SLOT_MAIN_HAND = 15,
  SLOT_OFF_HAND  = 16,
  SLOT_RANGED    = 17,
  SLOT_TABARD    = 18,
  SLOT_MAX       = 19,
  SLOT_MIN       = 0
};

// Tiers 13..16 + PVP
#define N_TIER 5
#define MIN_TIER ( 13 )

// Caster 2/4, Melee 2/4, Tank 2/4, Heal 2/4
#define N_TIER_BONUS 8

typedef uint32_t set_bonus_description_t[N_TIER][N_TIER_BONUS];

enum set_type_e
{
  SET_NONE = 0,
  SET_T13_CASTER, SET_T13_2PC_CASTER, SET_T13_4PC_CASTER,
  SET_T13_MELEE,  SET_T13_2PC_MELEE,  SET_T13_4PC_MELEE,
  SET_T13_TANK,   SET_T13_2PC_TANK,   SET_T13_4PC_TANK,
  SET_T13_HEAL,   SET_T13_2PC_HEAL,   SET_T13_4PC_HEAL,
  SET_T14_CASTER, SET_T14_2PC_CASTER, SET_T14_4PC_CASTER,
  SET_T14_MELEE,  SET_T14_2PC_MELEE,  SET_T14_4PC_MELEE,
  SET_T14_TANK,   SET_T14_2PC_TANK,   SET_T14_4PC_TANK,
  SET_T14_HEAL,   SET_T14_2PC_HEAL,   SET_T14_4PC_HEAL,
  SET_T15_CASTER, SET_T15_2PC_CASTER, SET_T15_4PC_CASTER,
  SET_T15_MELEE,  SET_T15_2PC_MELEE,  SET_T15_4PC_MELEE,
  SET_T15_TANK,   SET_T15_2PC_TANK,   SET_T15_4PC_TANK,
  SET_T15_HEAL,   SET_T15_2PC_HEAL,   SET_T15_4PC_HEAL,
  SET_T16_CASTER, SET_T16_2PC_CASTER, SET_T16_4PC_CASTER,
  SET_T16_MELEE,  SET_T16_2PC_MELEE,  SET_T16_4PC_MELEE,
  SET_T16_TANK,   SET_T16_2PC_TANK,   SET_T16_4PC_TANK,
  SET_T16_HEAL,   SET_T16_2PC_HEAL,   SET_T16_4PC_HEAL,
  SET_PVP_CASTER, SET_PVP_2PC_CASTER, SET_PVP_4PC_CASTER,
  SET_PVP_MELEE,  SET_PVP_2PC_MELEE,  SET_PVP_4PC_MELEE,
  SET_PVP_TANK,   SET_PVP_2PC_TANK,   SET_PVP_4PC_TANK,
  SET_PVP_HEAL,   SET_PVP_2PC_HEAL,   SET_PVP_4PC_HEAL,
  SET_MAX
};
//static_assert( static_cast<int>( SET_MAX ) == ( 1 + N_TIER * 3 * N_TIER_BONUS / 2 ), "enum set_type_e must be structured correctly." );

enum gem_type_e
{
  GEM_NONE=0,
  GEM_META, GEM_PRISMATIC,
  GEM_RED, GEM_YELLOW, GEM_BLUE,
  GEM_ORANGE, GEM_GREEN, GEM_PURPLE,
  GEM_COGWHEEL,
  GEM_MAX
};

enum meta_gem_type_e
{
  META_GEM_NONE=0,
  META_AGILE_SHADOWSPIRIT,
  META_AUSTERE_EARTHSIEGE,
  META_AUSTERE_SHADOWSPIRIT,
  META_BEAMING_EARTHSIEGE,
  META_BRACING_EARTHSIEGE,
  META_BRACING_EARTHSTORM,
  META_BRACING_SHADOWSPIRIT,
  META_BURNING_SHADOWSPIRIT,
  META_CHAOTIC_SHADOWSPIRIT,
  META_CHAOTIC_SKYFIRE,
  META_CHAOTIC_SKYFLARE,
  META_DESTRUCTIVE_SHADOWSPIRIT,
  META_DESTRUCTIVE_SKYFIRE,
  META_DESTRUCTIVE_SKYFLARE,
  META_EFFULGENT_SHADOWSPIRIT,
  META_EMBER_SHADOWSPIRIT,
  META_EMBER_SKYFIRE,
  META_EMBER_SKYFLARE,
  META_ENIGMATIC_SHADOWSPIRIT,
  META_ENIGMATIC_SKYFLARE,
  META_ENIGMATIC_STARFLARE,
  META_ENIGMATIC_SKYFIRE,
  META_ETERNAL_EARTHSIEGE,
  META_ETERNAL_EARTHSTORM,
  META_ETERNAL_SHADOWSPIRIT,
  META_FLEET_SHADOWSPIRIT,
  META_FORLORN_SHADOWSPIRIT,
  META_FORLORN_SKYFLARE,
  META_FORLORN_STARFLARE,
  META_IMPASSIVE_SHADOWSPIRIT,
  META_IMPASSIVE_SKYFLARE,
  META_IMPASSIVE_STARFLARE,
  META_INSIGHTFUL_EARTHSIEGE,
  META_INSIGHTFUL_EARTHSTORM,
  META_INVIGORATING_EARTHSIEGE,
  META_MYSTICAL_SKYFIRE,
  META_PERSISTENT_EARTHSIEGE,
  META_PERSISTENT_EARTHSHATTER,
  META_POWERFUL_EARTHSIEGE,
  META_POWERFUL_EARTHSHATTER,
  META_POWERFUL_EARTHSTORM,
  META_POWERFUL_SHADOWSPIRIT,
  META_RELENTLESS_EARTHSIEGE,
  META_RELENTLESS_EARTHSTORM,
  META_REVERBERATING_SHADOWSPIRIT,
  META_REVITALIZING_SHADOWSPIRIT,
  META_REVITALIZING_SKYFLARE,
  META_SWIFT_SKYFIRE,
  META_SWIFT_SKYFLARE,
  META_SWIFT_STARFIRE,
  META_SWIFT_STARFLARE,
  META_THUNDERING_SKYFIRE,
  META_THUNDERING_SKYFLARE,
  META_TIRELESS_STARFLARE,
  META_TIRELESS_SKYFLARE,
  META_TRENCHANT_EARTHSIEGE,
  META_TRENCHANT_EARTHSHATTER,
  META_GEM_MAX
};

enum stat_type_e
{
  STAT_NONE=0,
  STAT_STRENGTH, STAT_AGILITY, STAT_STAMINA, STAT_INTELLECT, STAT_SPIRIT,
  STAT_HEALTH, STAT_MANA, STAT_RAGE, STAT_ENERGY, STAT_FOCUS, STAT_RUNIC,
  STAT_MAX_HEALTH, STAT_MAX_MANA, STAT_MAX_RAGE, STAT_MAX_ENERGY, STAT_MAX_FOCUS, STAT_MAX_RUNIC,
  STAT_SPELL_POWER, STAT_MP5,
  STAT_ATTACK_POWER, STAT_EXPERTISE_RATING, STAT_EXPERTISE_RATING2,
  STAT_HIT_RATING, STAT_HIT_RATING2,STAT_CRIT_RATING, STAT_HASTE_RATING,STAT_MASTERY_RATING,
  STAT_WEAPON_DPS, STAT_WEAPON_SPEED,
  STAT_WEAPON_OFFHAND_DPS, STAT_WEAPON_OFFHAND_SPEED,
  STAT_ARMOR, STAT_BONUS_ARMOR, STAT_RESILIENCE_RATING, STAT_DODGE_RATING, STAT_PARRY_RATING,
  STAT_BLOCK_RATING,
  STAT_ALL,
  STAT_MAX
};
#define check(x) static_assert( static_cast<int>( STAT_##x ) == static_cast<int>( ATTR_##x ), \
                                "stat_type_e and attribute_type_e must be kept in sync" )
check( STRENGTH );
check( AGILITY );
check( STAMINA );
check( INTELLECT );
check( SPIRIT );
#undef check

inline stat_type_e stat_from_attr( attribute_type_e a )
{
  // Assumes that ATTR_X == STAT_X
  return static_cast<stat_type_e>( a );
}

enum elixir_type_e
{
  ELIXIR_NONE=0,
  ELIXIR_ADEPT,
  ELIXIR_BATTLE,
  ELIXIR_DRAENIC_WISDOM,
  ELIXIR_FEL_STRENGTH,
  ELIXIR_GREATER_ARCANE,
  ELIXIR_GUARDIAN,
  ELIXIR_MAJOR_AGILITY,
  ELIXIR_MAJOR_FIRE_POWER,
  ELIXIR_MAJOR_FROST_POWER,
  ELIXIR_MAJOR_MAGEBLOOD,
  ELIXIR_MAJOR_SHADOW_POWER,
  ELIXIR_MAJOR_STRENGTH,
  ELIXIR_MASTERY,
  ELIXIR_MONGOOSE,
  ELIXIR_ONSLAUGHT,
  ELIXIR_SAGES,
  ELIXIR_MAX
};

enum flask_type_e
{
  FLASK_NONE=0,
  FLASK_BLINDING_LIGHT,
  FLASK_DISTILLED_WISDOM,
  FLASK_DRACONIC_MIND,
  FLASK_ENDLESS_RAGE,
  FLASK_ENHANCEMENT,
  FLASK_FLOWING_WATER,
  FLASK_FROST_WYRM,
  FLASK_MIGHTY_RESTORATION,
  FLASK_NORTH,
  FLASK_PURE_DEATH,
  FLASK_PURE_MOJO,
  FLASK_RELENTLESS_ASSAULT,
  FLASK_SUPREME_POWER,
  FLASK_STEELSKIN,
  FLASK_TITANIC_STRENGTH,
  FLASK_WINDS,
  FLASK_MAX
};

enum food_type_e
{
  FOOD_NONE=0,
  FOOD_BAKED_ROCKFISH,
  FOOD_BASILISK_LIVERDOG,
  FOOD_BEER_BASTED_CROCOLISK,
  FOOD_BLACKBELLY_SUSHI,
  FOOD_BLACKENED_BASILISK,
  FOOD_BLACKENED_DRAGONFIN,
  FOOD_CROCOLISK_AU_GRATIN,
  FOOD_CRUNCHY_SERPENT,
  FOOD_DELICIOUS_SAGEFISH_TAIL,
  FOOD_DRAGONFIN_FILET,
  FOOD_FISH_FEAST,
  FOOD_FORTUNE_COOKIE,
  FOOD_GOLDEN_FISHSTICKS,
  FOOD_GREAT_FEAST,
  FOOD_GRILLED_DRAGON,
  FOOD_HEARTY_RHINO,
  FOOD_IMPERIAL_MANTA_STEAK,
  FOOD_LAVASCALE_FILLET,
  FOOD_MEGA_MAMMOTH_MEAL,
  FOOD_MUSHROOM_SAUCE_MUDFISH,
  FOOD_POACHED_BLUEFISH,
  FOOD_POACHED_NORTHERN_SCULPIN,
  FOOD_RHINOLICIOUS_WORMSTEAK,
  FOOD_SEAFOOD_MAGNIFIQUE_FEAST,
  FOOD_SEVERED_SAGEFISH_HEAD,
  FOOD_SKEWERED_EEL,
  FOOD_SMOKED_SALMON,
  FOOD_SNAPPER_EXTREME,
  FOOD_TENDER_SHOVELTUSK_STEAK,
  FOOD_VERY_BURNT_WORG,
  FOOD_MAX
};

enum position_type_e { POSITION_NONE=0, POSITION_FRONT, POSITION_BACK, POSITION_RANGED_FRONT, POSITION_RANGED_BACK, POSITION_MAX };

enum profession_type_e
{
  PROFESSION_NONE=0,
  PROF_ALCHEMY,
  PROF_MINING,
  PROF_HERBALISM,
  PROF_LEATHERWORKING,
  PROF_ENGINEERING,
  PROF_BLACKSMITHING,
  PROF_INSCRIPTION,
  PROF_SKINNING,
  PROF_TAILORING,
  PROF_JEWELCRAFTING,
  PROF_ENCHANTING,
  PROFESSION_MAX
};

enum role_type_e { ROLE_NONE=0, ROLE_ATTACK, ROLE_SPELL, ROLE_HYBRID, ROLE_DPS, ROLE_TANK, ROLE_HEAL, ROLE_MAX };

enum rng_type_e
{
  // Specifies the general class of RNG desired
  RNG_DEFAULT=0,     // Do not care/know where it will be used
  RNG_GLOBAL,        // Returns reference to global RNG on sim_t
  RNG_DETERMINISTIC, // Returns reference to global deterministic RNG on sim_t

  // Specifies a particular RNG desired
  RNG_STANDARD,          // Creates RNG using srand() and rand()
  RNG_MERSENNE_TWISTER,  // Creates RNG using SIMD oriented Fast Mersenne Twister
  RNG_MAX
};

enum save_type_e
{
  // Specifies the type of profile data to be saved
  SAVE_ALL=0,
  SAVE_GEAR,
  SAVE_TALENTS,
  SAVE_ACTIONS,
  SAVE_MAX
};

enum format_type_e
{
  FORMAT_NONE=0,
  FORMAT_NAME,
  FORMAT_CHAR_NAME,
  FORMAT_CONVERT_HEX,
  FORMAT_CONVERT_UTF8,
  FORMAT_MAX
};

#define FORMAT_CHAR_NAME_MASK  ( (1<<FORMAT_NAME) | (1<<FORMAT_CHAR_NAME) )
#define FORMAT_GUILD_NAME_MASK ( (1<<FORMAT_NAME) )
#define FORMAT_ALL_NAME_MASK   ( (1<<FORMAT_NAME) | (1<<FORMAT_CHAR_NAME) )
#define FORMAT_UTF8_MASK       ( (1<<FORMAT_CONVERT_HEX) | (1<<FORMAT_CONVERT_UTF8) )
#define FORMAT_ASCII_MASK      ( (1<<FORMAT_CONVERT_UTF8) )
#define FORMAT_CONVERT_MASK    ( (1<<FORMAT_CONVERT_HEX) | (1<<FORMAT_CONVERT_UTF8) )
#define FORMAT_DEFAULT         ( FORMAT_ASCII_MASK )
#define FORMAT_ALL_MASK        -1

// Data Access ==============================================================
#ifndef MAX_LEVEL
#define MAX_LEVEL (90)
#endif

#ifndef NUM_SPELL_FLAGS
#define NUM_SPELL_FLAGS (10)
#endif

enum power_type_e
{
  POWER_HEALTH        = -2,
  POWER_MANA          = 0,
  POWER_RAGE          = 1,
  POWER_FOCUS         = 2,
  POWER_ENERGY        = 3,
  POWER_MONK_ENERGY   = 4,
  POWER_RUNE          = 5,
  POWER_RUNIC_POWER   = 6,
  POWER_SOUL_SHARDS   = 7,
  // Not yet used
  POWER_HOLY_POWER    = 9,
  // Not yet used (MoP Monk deprecated resource #1)
  // Not yet used
  POWER_CHI           = 12,
  POWER_SHADOW_ORB    = 13,
  POWER_BURNING_EMBER = 14,
  POWER_DEMONIC_FURY  = 15,
  // Helpers
  POWER_MAX           = 16,
  POWER_NONE          = 0xFFFFFFFF, // None.
  POWER_OFFSET        = 2,
};

enum rating_type_e
{
  RATING_DODGE = 0,
  RATING_PARRY,
  RATING_BLOCK,
  RATING_MELEE_HIT,
  RATING_RANGED_HIT,
  RATING_SPELL_HIT,
  RATING_MELEE_CRIT,
  RATING_RANGED_CRIT,
  RATING_SPELL_CRIT,
  RATING_MELEE_HASTE,
  RATING_RANGED_HASTE,
  RATING_SPELL_HASTE,
  RATING_EXPERTISE,
  RATING_MASTERY,
  RATING_MAX
};

// New stuff
enum snapshot_state_e
{
  STATE_HASTE         = 0x000001,
  STATE_CRIT          = 0x000002,
  STATE_AP            = 0x000004,
  STATE_SP            = 0x000008,

  STATE_MUL_DA        = 0x000010,
  STATE_MUL_TA        = 0x000020,
  STATE_MUL_TARGET    = 0x000040,
};

// Generic programming tools ================================================

template <typename T, std::size_t N>
inline std::size_t sizeof_array( const T ( & )[N] )
{ return N; }

template <typename T, std::size_t N>
inline std::size_t sizeof_array( const std::array<T,N>& )
{ return N; }

class noncopyable
{
public:
  noncopyable() {} // = default
  // noncopyable( noncopyable&& ) = default;
  // noncopyable& operator = ( noncopyable&& ) = default;
private:
  noncopyable( const noncopyable& ); // = delete
  noncopyable& operator = ( const noncopyable& ); // = delete
};

class nonmoveable : public noncopyable
{
private:
  // nonmoveable( nonmoveable&& ) = delete;
  // nonmoveable& operator = ( nonmoveable&& ) = delete;
};

// Adapted from (read "stolen") boost::checked_deleter
struct delete_disposer_t
{
  template <typename T>
  void operator () ( T* t ) const
  {
    typedef int force_T_to_be_complete[ sizeof( T ) ? 1 : -1 ];
    ( void )sizeof( force_T_to_be_complete );
    delete t;
  }
};

// Generic algorithms =======================================================

// Wrappers for std::fill, std::fill_n, and std::find that perform any type
// conversions for t at the callsite instead of per assignment in the loop body.
// (i.e., fill( T**, T**, 0 ) will deduce T* for the third argument, as opposed
//  to std::fill( T**, T**, 0 ) simply defaulting to int and failing to compile).
template <typename I>
inline void fill( I first, I last, typename std::iterator_traits<I>::value_type const& t )
{ std::fill( first, last, t ); }

template <typename I>
inline void fill_n( I first, typename std::iterator_traits<I>::difference_type n,
                    typename std::iterator_traits<I>::value_type const& t )
{ std::fill_n( first, n, t ); }

template <typename I>
inline I find( I first, I last, typename std::iterator_traits<I>::value_type const& t )
{ return std::find( first, last, t ); }

template <typename I, typename D>
void dispose( I first, I last, D disposer )
{
  while ( first != last )
    disposer( *first++ );
}

template <typename I>
inline void dispose( I first, I last )
{ dispose( first, last, delete_disposer_t() ); }

template <unsigned HW, typename Fwd, typename Out>
void sliding_window_average( Fwd first, Fwd last, Out out )
{
  typedef typename std::iterator_traits<Fwd>::value_type value_t;
  typedef typename std::iterator_traits<Fwd>::difference_type diff_t;
  const diff_t n = std::distance( first, last );
  const diff_t HALFWINDOW = static_cast<diff_t>( HW );

  if ( n >= 2 * HALFWINDOW )
  {
    value_t window_sum = value_t();

    // Fill right half of sliding window
    Fwd right = first;
    for ( diff_t count = 0; count < HALFWINDOW; ++count )
      window_sum += *right++;

    // Fill left half of sliding window
    for ( diff_t count = HALFWINDOW; count < 2 * HALFWINDOW; ++count )
    {
      window_sum += *right++;
      *out++ = window_sum / ( count + 1 );
    }

    // Slide until window hits end of data
    while ( right != last )
    {
      window_sum += *right++;
      *out++ = window_sum / ( 2 * HALFWINDOW + 1 );
      window_sum -= *first++;
    }

    // Empty right half of sliding window
    for ( diff_t count = 2 * HALFWINDOW; count > HALFWINDOW; --count )
    {
      *out++ = window_sum / count;
      window_sum -= *first++;
    }
  }
  else
  {
    // input is pathologically small compared to window size, just average everything.
    fill_n( out, n, std::accumulate( first, last, value_t() ) / n );
  }
}

// Machinery for range-based generic algorithms =============================

namespace range { // ========================================================
template <typename T>
struct traits
{
  typedef typename iterator_type<T>::type iterator;
  static iterator begin( T& t ) { return t.begin(); }
  static iterator end( T& t ) { return t.end(); }
};

template <typename T, size_t N>
struct traits<T[N]>
{
  typedef T* iterator;
  static iterator begin( T ( &t )[N] ) { return &t[0]; }
  static iterator end( T ( &t )[N] ) { return begin( t ) + N; }
};

template <typename T>
struct traits< std::pair<T,T> >
{
  typedef T iterator;
  static iterator begin( const std::pair<T,T>& t ) { return t.first; }
  static iterator end( const std::pair<T,T>& t ) { return t.second; }
};

template <typename T>
struct traits< const std::pair<T,T> >
{
  typedef T iterator;
  static iterator begin( const std::pair<T,T>& t ) { return t.first; }
  static iterator end( const std::pair<T,T>& t ) { return t.second; }
};

template <typename T>
struct value_type
{
  typedef typename std::iterator_traits<typename traits<T>::iterator>::value_type type;
};

template <typename T>
inline typename traits<T>::iterator begin( T& t )
{ return traits<T>::begin( t ); }

template <typename T>
inline typename traits<const T>::iterator cbegin( const T& t )
{ return range::begin( t ); }

template <typename T>
inline typename traits<T>::iterator end( T& t )
{ return traits<T>::end( t ); }

template <typename T>
inline typename traits<const T>::iterator cend( const T& t )
{ return range::end( t ); }

// Range-based generic algorithms ===========================================

#if _MSC_VER && _MSC_VER < 1600
#pragma warning( disable : 4996 )
#endif
template <typename Range, typename Out>
inline Out copy( const Range& r, Out o )
{ return std::copy( range::begin( r ), range::end( r ), o ); }
#if _MSC_VER && _MSC_VER < 1600
#pragma warning( default : 4996 )
#endif

template <typename Range, typename D>
inline Range& dispose( Range& r, D disposer )
{ dispose( range::begin( r ), range::end( r ), disposer ); return r; }

template <typename Range>
inline Range& dispose( Range& r )
{ return dispose( r, delete_disposer_t() ); }

template <typename Range>
inline Range& fill( Range& r, typename range::value_type<Range>::type const& t )
{ std::fill( range::begin( r ), range::end( r ), t ); return r; }

template <typename Range>
inline typename range::traits<Range>::iterator
find( Range& r, typename range::value_type<Range>::type const& t )
{ return std::find( range::begin( r ), range::end( r ), t ); }

template <typename Range, typename F>
inline F for_each( Range& r, F f )
{ return std::for_each( range::begin( r ), range::end( r ), f ); }

template <typename Range, typename Out, typename Predicate>
inline Out remove_copy_if( Range& r, Out o, Predicate p )
{ return std::remove_copy_if( range::begin( r ), range::end( r ), o, p ); }

template <typename Range1, typename Range2, typename Out>
inline Out set_difference( const Range1& left, const Range2& right, Out o )
{
  return std::set_difference( range::begin( left ), range::end( left ),
                              range::begin( right ), range::end( right ), o );
}

template <typename Range1, typename Range2, typename Out, typename Compare>
inline Out set_difference( const Range1& left, const Range2& right, Out o, Compare c )
{
  return std::set_difference( range::begin( left ), range::end( left ),
                              range::begin( right ), range::end( right ), o, c );
}

template <typename Range1, typename Range2, typename Out>
inline Out set_intersection( const Range1& left, const Range2& right, Out o )
{
  return std::set_intersection( range::begin( left ), range::end( left ),
                                range::begin( right ), range::end( right ), o );
}

template <typename Range1, typename Range2, typename Out, typename Compare>
inline Out set_intersection( const Range1& left, const Range2& right, Out o, Compare c )
{
  return std::set_intersection( range::begin( left ), range::end( left ),
                                range::begin( right ), range::end( right ), o, c );
}

template <typename Range1, typename Range2, typename Out>
inline Out set_union( const Range1& left, const Range2& right, Out o )
{
  return std::set_union( range::begin( left ), range::end( left ),
                         range::begin( right ), range::end( right ), o );
}

template <typename Range1, typename Range2, typename Out, typename Compare>
inline Out set_union( const Range1& left, const Range2& right, Out o, Compare c )
{
  return std::set_union( range::begin( left ), range::end( left ),
                         range::begin( right ), range::end( right ), o, c );
}

template <unsigned HW, typename Range, typename Out>
inline Range& sliding_window_average( Range& r, Out out )
{ ::sliding_window_average<HW>( range::begin( r ), range::end( r ), out ); return r; }

template <typename Range>
inline Range& sort( Range& r )
{ std::sort( range::begin( r ), range::end( r ) ); return r; }

template <typename Range, typename Comp>
inline Range& sort( Range& r, Comp c )
{ std::sort( range::begin( r ), range::end( r ), c ); return r; }

template <typename Range>
inline typename range::traits<Range>::iterator unique( Range& r )
{ return std::unique( range::begin( r ), range::end( r ) ); }

template <typename Range, typename Comp>
inline typename range::traits<Range>::iterator unique( Range& r, Comp c )
{ return std::unique( range::begin( r ), range::end( r ), c ); }

template <typename Range>
inline bool is_valid_utf8( const Range& r )
{ return utf8::is_valid( range::begin( r ), range::end( r ) ); }

template <typename Range>
inline typename range::traits<Range>::iterator max_element( Range& r )
{ return std::max_element( range::begin( r ), range::end( r ) ); }

} // namespace range ========================================================

// Adapter for container of owned pointers; automatically deletes the
// pointers on destruction.
template <typename Container>
class auto_dispose : public Container
{
private:
  void dispose_()
  { range::dispose( *this ); }

public:
  ~auto_dispose() { dispose_(); }

  using Container::clear;

  void dispose()
  { dispose_(); clear(); }
};

// timespan_t ===============================================================

// if timespan_t is in the global namespace, there's a name lookup issue with
// one of the Qt headers. Problem is avoided by defining in a sub-namespace
// and then lifting into the global namespace with a using declaration.
namespace timespan_adl_barrier { // =========================================

class timespan_t
{
#ifdef SC_USE_INTEGER_TIME
  // CAREFUL: Using int32_t implies that no overflowing happens during calculation.
  typedef int64_t time_t;

  static time_t native_to_milli ( time_t t ) { return t; }
  static double native_to_second( time_t t ) { return static_cast<double>( t ) * ( 1.0 / 1000 ); }
  static double native_to_minute( time_t t ) { return static_cast<double>( t ) * ( 1.0 / ( 60 * 1000 ) ); }

  template <typename Rep>
  static time_t milli_to_native ( Rep t ) { return static_cast<time_t>( t ); }
  template <typename Rep>
  static time_t second_to_native( Rep t ) { return static_cast<time_t>( t * 1000 ); }
  template <typename Rep>
  static time_t minute_to_native( Rep t ) { return static_cast<time_t>( t * ( 60 * 1000 ) ); }

#else // !SC_USE_INTEGER_TIME

  typedef double time_t;

  static time_t native_to_milli ( time_t t ) { return t * 1000.0; }
  static double native_to_second( time_t t ) { return t; }
  static double native_to_minute( time_t t ) { return t * ( 1.0 / 60 ); }

  template <typename Rep>
  static time_t milli_to_native ( Rep t ) { return static_cast<time_t>( t / 1000.0 ); }
  template <typename Rep>
  static time_t second_to_native( Rep t ) { return static_cast<time_t>( t ); }
  template <typename Rep>
  static time_t minute_to_native( Rep t ) { return static_cast<time_t>( t * 60 ); }
#endif

  time_t time;

  template <typename Rep>
  explicit timespan_t( Rep t ) : time( static_cast<time_t>( t ) ) {}

public:
  double total_minutes() const { return native_to_minute( time ); }
  double total_seconds() const { return native_to_second( time ); }
  time_t total_millis() const  { return native_to_milli ( time ); }

  template <typename Rep>
  static typename enable_if<std::is_arithmetic<Rep>::value,timespan_t>::type
  from_millis ( Rep millis )  { return timespan_t( milli_to_native ( millis ) ); }

  template <typename Rep>
  static typename enable_if<std::is_arithmetic<Rep>::value,timespan_t>::type
  from_seconds( Rep seconds ) { return timespan_t( second_to_native( seconds ) ); }

  template <typename Rep>
  static typename enable_if<std::is_arithmetic<Rep>::value,timespan_t>::type
  from_minutes( Rep minutes ) { return timespan_t( minute_to_native( minutes ) ); }

  timespan_t() {}

  bool operator==( timespan_t right ) const { return time == right.time; }
  bool operator!=( timespan_t right ) const { return time != right.time; }

  bool operator> ( timespan_t right ) const { return time >  right.time; }
  bool operator>=( timespan_t right ) const { return time >= right.time; }
  bool operator< ( timespan_t right ) const { return time <  right.time; }
  bool operator<=( timespan_t right ) const { return time <= right.time; }

  timespan_t& operator+=( timespan_t right )
  {
    time += right.time;
    return *this;
  }
  timespan_t& operator-=( timespan_t right )
  {
    time -= right.time;
    return *this;
  }

  template <typename Rep>
  typename enable_if<std::is_arithmetic<Rep>::value,timespan_t&>::type
  operator*=( Rep right )
  {
    time = static_cast<time_t>( time * right );
    return *this;
  }

  template <typename Rep>
  typename enable_if<std::is_arithmetic<Rep>::value,timespan_t&>::type
  operator/=( Rep right )
  {
    time = static_cast<time_t>( time / right );
    return *this;
  }

  friend timespan_t operator+( timespan_t right )
  { return right; }
  friend timespan_t operator-( timespan_t right )
  { return timespan_t( -right.time ); }

  friend timespan_t operator+( timespan_t left, timespan_t right )
  { return timespan_t( left.time + right.time ); }

  friend timespan_t operator-( timespan_t left, timespan_t right )
  { return timespan_t( left.time - right.time ); }

  template <typename Rep>
  friend typename enable_if<std::is_arithmetic<Rep>::value,timespan_t>::type
  operator*( timespan_t left, Rep right )
  { return timespan_t( left.time * right ); }

  template <typename Rep>
  friend typename enable_if<std::is_arithmetic<Rep>::value,timespan_t>::type
  operator*( Rep left, timespan_t right )
  { return timespan_t( left * right.time ); }

  template <typename Rep>
  friend typename enable_if<std::is_arithmetic<Rep>::value,timespan_t>::type
  operator/( timespan_t left, Rep right )
  { return timespan_t( left.time / right ); }

  friend double operator/( timespan_t left, timespan_t right )
  { return static_cast<double>( left.time ) / right.time; }

  typedef time_t native_t;
  static native_t to_native( timespan_t t ) { return t.time; }
  template <typename Rep>
  static timespan_t from_native( Rep t ) { return timespan_t( static_cast<time_t>( t ) ); }

  static timespan_t zero() { return timespan_t( 0 ); }
  static timespan_t max() { return timespan_t( std::numeric_limits<time_t>::max() ); }
  static timespan_t min()
  {
    if ( std::is_floating_point<time_t>::value )
      return timespan_t( -std::numeric_limits<time_t>::max() );
    else
      return timespan_t( std::numeric_limits<time_t>::min() );
  }
};

} // namespace timespan_adl_barrier =========================================

using timespan_adl_barrier::timespan_t;

// Cache Control ============================================================

namespace cache {

typedef int era_t;
static const era_t INVALID_ERA = -1;
static const era_t IN_THE_BEGINNING = 0;  // A time before any other possible era;
                                          // used to mark persistent caches at load.

enum behavior_e
{
  ANY,      // * Use any version present in the cache, retrieve if not present.
  CURRENT,  // * Use only current info from the cache; validate old versions as needed.
  ONLY,     // * Use any version present in the cache, fail if not present.
};

class cache_control_t
{
private:
  era_t current_era;
  behavior_e player_cache_behavior;
  behavior_e item_cache_behavior;

public:
  cache_control_t() :
    current_era( IN_THE_BEGINNING ),
    player_cache_behavior( CURRENT ),
    item_cache_behavior( ANY )
  {}

  era_t era() const { return current_era; }
  void advance_era() { ++current_era; }

  behavior_e cache_players() const { return player_cache_behavior; }
  void cache_players( behavior_e b ) { player_cache_behavior = b; }

  behavior_e cache_items() const { return item_cache_behavior; }
  void cache_items( behavior_e b ) { item_cache_behavior = b; }

  static cache_control_t singleton;
};

// Caching system's global notion of the current time.
inline era_t era()
{ return cache_control_t::singleton.era(); }

// Time marches on.
inline void advance_era()
{ cache_control_t::singleton.advance_era(); }

// Get/Set default cache behaviors.
inline behavior_e players()
{ return cache_control_t::singleton.cache_players(); }
inline void players( behavior_e b )
{ cache_control_t::singleton.cache_players( b ); }

inline behavior_e items()
{ return cache_control_t::singleton.cache_items(); }
inline void items( behavior_e b )
{ cache_control_t::singleton.cache_items( b ); }

}

struct stat_data_t
{
  double strength;
  double agility;
  double stamina;
  double intellect;
  double spirit;
  double base_health;
  double base_resource;
};

#define DEFAULT_MISC_VALUE std::numeric_limits<int>::min()

// These names come from the MANGOS project.
enum spell_attribute_e
{
  SPELL_ATTR_UNK0 = 0x0000, // 0
  SPELL_ATTR_RANGED, // 1 All ranged abilites have this flag
  SPELL_ATTR_ON_NEXT_SWING_1, // 2 on next swing
  SPELL_ATTR_UNK3, // 3 not set in 3.0.3
  SPELL_ATTR_UNK4, // 4 isAbility
  SPELL_ATTR_TRADESPELL, // 5 trade spells, will be added by client to a sublist of profession spell
  SPELL_ATTR_PASSIVE, // 6 Passive spell
  SPELL_ATTR_UNK7, // 7 can't be linked in chat?
  SPELL_ATTR_UNK8, // 8 hide created item in tooltip (for effect=24)
  SPELL_ATTR_UNK9, // 9
  SPELL_ATTR_ON_NEXT_SWING_2, // 10 on next swing 2
  SPELL_ATTR_UNK11, // 11
  SPELL_ATTR_DAYTIME_ONLY, // 12 only useable at daytime, not set in 2.4.2
  SPELL_ATTR_NIGHT_ONLY, // 13 only useable at night, not set in 2.4.2
  SPELL_ATTR_INDOORS_ONLY, // 14 only useable indoors, not set in 2.4.2
  SPELL_ATTR_OUTDOORS_ONLY, // 15 Only useable outdoors.
  SPELL_ATTR_NOT_SHAPESHIFT, // 16 Not while shapeshifted
  SPELL_ATTR_ONLY_STEALTHED, // 17 Must be in stealth
  SPELL_ATTR_UNK18, // 18
  SPELL_ATTR_LEVEL_DAMAGE_CALCULATION, // 19 spelldamage depends on caster level
  SPELL_ATTR_STOP_ATTACK_TARGE, // 20 Stop attack after use this spell (and not begin attack if use)
  SPELL_ATTR_IMPOSSIBLE_DODGE_PARRY_BLOCK, // 21 Cannot be dodged/parried/blocked
  SPELL_ATTR_UNK22, // 22
  SPELL_ATTR_UNK23, // 23 castable while dead?
  SPELL_ATTR_CASTABLE_WHILE_MOUNTED, // 24 castable while mounted
  SPELL_ATTR_DISABLED_WHILE_ACTIVE, // 25 Activate and start cooldown after aura fade or remove summoned creature or go
  SPELL_ATTR_UNK26, // 26
  SPELL_ATTR_CASTABLE_WHILE_SITTING, // 27 castable while sitting
  SPELL_ATTR_CANT_USED_IN_COMBAT, // 28 Cannot be used in combat
  SPELL_ATTR_UNAFFECTED_BY_INVULNERABILITY, // 29 unaffected by invulnerability (hmm possible not...)
  SPELL_ATTR_UNK30, // 30 breakable by damage?
  SPELL_ATTR_CANT_CANCEL, // 31 positive aura can't be canceled

  SPELL_ATTR_EX_UNK0 = 0x0100, // 0
  SPELL_ATTR_EX_DRAIN_ALL_POWER, // 1 use all power (Only paladin Lay of Hands and Bunyanize)
  SPELL_ATTR_EX_CHANNELED_1, // 2 channeled 1
  SPELL_ATTR_EX_UNK3, // 3
  SPELL_ATTR_EX_UNK4, // 4
  SPELL_ATTR_EX_NOT_BREAK_STEALTH, // 5 Not break stealth
  SPELL_ATTR_EX_CHANNELED_2, // 6 channeled 2
  SPELL_ATTR_EX_NEGATIVE, // 7
  SPELL_ATTR_EX_NOT_IN_COMBAT_TARGET, // 8 Spell req target not to be in combat state
  SPELL_ATTR_EX_UNK9, // 9
  SPELL_ATTR_EX_NO_INITIAL_AGGRO, // 10 no generates threat on cast 100%
  SPELL_ATTR_EX_UNK11, // 11
  SPELL_ATTR_EX_UNK12, // 12
  SPELL_ATTR_EX_UNK13, // 13
  SPELL_ATTR_EX_UNK14, // 14
  SPELL_ATTR_EX_DISPEL_AURAS_ON_IMMUNITY, // 15 remove auras on immunity
  SPELL_ATTR_EX_UNAFFECTED_BY_SCHOOL_IMMUNE, // 16 unaffected by school immunity
  SPELL_ATTR_EX_UNK17, // 17 for auras AURA_TRACK_CREATURES, AURA_TRACK_RESOURCES and AURA_TRACK_STEALTHED select non-stacking tracking spells
  SPELL_ATTR_EX_UNK18, // 18
  SPELL_ATTR_EX_UNK19, // 19
  SPELL_ATTR_EX_REQ_COMBO_POINTS1, // 20 Req combo points on target
  SPELL_ATTR_EX_UNK21, // 21
  SPELL_ATTR_EX_REQ_COMBO_POINTS2, // 22 Req combo points on target
  SPELL_ATTR_EX_UNK23, // 23
  SPELL_ATTR_EX_UNK24, // 24 Req fishing pole??
  SPELL_ATTR_EX_UNK25, // 25
  SPELL_ATTR_EX_UNK26, // 26
  SPELL_ATTR_EX_UNK27, // 27
  SPELL_ATTR_EX_UNK28, // 28
  SPELL_ATTR_EX_UNK29, // 29
  SPELL_ATTR_EX_UNK30, // 30 overpower
  SPELL_ATTR_EX_UNK31, // 31

  SPELL_ATTR_EX2_UNK0 = 0x0200, // 0
  SPELL_ATTR_EX2_UNK1, // 1
  SPELL_ATTR_EX2_CANT_REFLECTED, // 2 ? used for detect can or not spell reflected // do not need LOS (e.g. 18220 since 3.3.3)
  SPELL_ATTR_EX2_UNK3, // 3 auto targeting? (e.g. fishing skill enhancement items since 3.3.3)
  SPELL_ATTR_EX2_UNK4, // 4
  SPELL_ATTR_EX2_AUTOREPEAT_FLAG, // 5
  SPELL_ATTR_EX2_UNK6, // 6 only usable on tabbed by yourself
  SPELL_ATTR_EX2_UNK7, // 7
  SPELL_ATTR_EX2_UNK8, // 8 not set in 3.0.3
  SPELL_ATTR_EX2_UNK9, // 9
  SPELL_ATTR_EX2_UNK10, // 10
  SPELL_ATTR_EX2_HEALTH_FUNNEL, // 11
  SPELL_ATTR_EX2_UNK12, // 12
  SPELL_ATTR_EX2_UNK13, // 13
  SPELL_ATTR_EX2_UNK14, // 14
  SPELL_ATTR_EX2_UNK15, // 15 not set in 3.0.3
  SPELL_ATTR_EX2_UNK16, // 16
  SPELL_ATTR_EX2_UNK17, // 17 suspend weapon timer instead of resetting it, (?Hunters Shot and Stings only have this flag?)
  SPELL_ATTR_EX2_UNK18, // 18 Only Revive pet - possible req dead pet
  SPELL_ATTR_EX2_NOT_NEED_SHAPESHIFT, // 19 does not necessarly need shapeshift
  SPELL_ATTR_EX2_UNK20, // 20
  SPELL_ATTR_EX2_DAMAGE_REDUCED_SHIELD, // 21 for ice blocks, pala immunity buffs, priest absorb shields, but used also for other spells -> not sure!
  SPELL_ATTR_EX2_UNK22, // 22
  SPELL_ATTR_EX2_UNK23, // 23 Only mage Arcane Concentration have this flag
  SPELL_ATTR_EX2_UNK24, // 24
  SPELL_ATTR_EX2_UNK25, // 25
  SPELL_ATTR_EX2_UNK26, // 26 unaffected by school immunity
  SPELL_ATTR_EX2_UNK27, // 27
  SPELL_ATTR_EX2_UNK28, // 28 no breaks stealth if it fails??
  SPELL_ATTR_EX2_CANT_CRIT, // 29 Spell can't crit
  SPELL_ATTR_EX2_UNK30, // 30
  SPELL_ATTR_EX2_FOOD_BUFF, // 31 Food or Drink Buff (like Well Fed)

  SPELL_ATTR_EX3_UNK0 = 0x0300, // 0
  SPELL_ATTR_EX3_UNK1, // 1
  SPELL_ATTR_EX3_UNK2, // 2
  SPELL_ATTR_EX3_UNK3, // 3
  SPELL_ATTR_EX3_UNK4, // 4 Druid Rebirth only this spell have this flag
  SPELL_ATTR_EX3_UNK5, // 5
  SPELL_ATTR_EX3_UNK6, // 6
  SPELL_ATTR_EX3_UNK7, // 7 create a separate (de)buff stack for each caster
  SPELL_ATTR_EX3_UNK8, // 8
  SPELL_ATTR_EX3_UNK9, // 9
  SPELL_ATTR_EX3_MAIN_HAND, // 10 Main hand weapon required
  SPELL_ATTR_EX3_BATTLEGROUND, // 11 Can casted only on battleground
  SPELL_ATTR_EX3_CAST_ON_DEAD, // 12 target is a dead player (not every spell has this flag)
  SPELL_ATTR_EX3_UNK13, // 13
  SPELL_ATTR_EX3_UNK14, // 14 "Honorless Target" only this spells have this flag
  SPELL_ATTR_EX3_UNK15, // 15 Auto Shoot, Shoot, Throw, - this is autoshot flag
  SPELL_ATTR_EX3_UNK16, // 16 no triggers effects that trigger on casting a spell??
  SPELL_ATTR_EX3_UNK17, // 17 no triggers effects that trigger on casting a spell??
  SPELL_ATTR_EX3_UNK18, // 18
  SPELL_ATTR_EX3_UNK19, // 19
  SPELL_ATTR_EX3_DEATH_PERSISTENT, // 20 Death persistent spells
  SPELL_ATTR_EX3_UNK21, // 21
  SPELL_ATTR_EX3_REQ_WAND, // 22 Req wand
  SPELL_ATTR_EX3_UNK23, // 23
  SPELL_ATTR_EX3_REQ_OFFHAND, // 24 Req offhand weapon
  SPELL_ATTR_EX3_UNK25, // 25 no cause spell pushback ?
  SPELL_ATTR_EX3_UNK26, // 26
  SPELL_ATTR_EX3_UNK27, // 27
  SPELL_ATTR_EX3_UNK28, // 28
  SPELL_ATTR_EX3_UNK29, // 29
  SPELL_ATTR_EX3_UNK30, // 30
  SPELL_ATTR_EX3_UNK31, // 31

  SPELL_ATTR_EX4_UNK0 = 0x0400, // 0
  SPELL_ATTR_EX4_UNK1, // 1 proc on finishing move?
  SPELL_ATTR_EX4_UNK2, // 2
  SPELL_ATTR_EX4_UNK3, // 3
  SPELL_ATTR_EX4_UNK4, // 4 This will no longer cause guards to attack on use??
  SPELL_ATTR_EX4_UNK5, // 5
  SPELL_ATTR_EX4_NOT_STEALABLE, // 6 although such auras might be dispellable, they cannot be stolen
  SPELL_ATTR_EX4_UNK7, // 7
  SPELL_ATTR_EX4_STACK_DOT_MODIFIER, // 8 no effect on non DoTs?
  SPELL_ATTR_EX4_UNK9, // 9
  SPELL_ATTR_EX4_SPELL_VS_EXTEND_COST, // 10 Rogue Shiv have this flag
  SPELL_ATTR_EX4_UNK11, // 11
  SPELL_ATTR_EX4_UNK12, // 12
  SPELL_ATTR_EX4_UNK13, // 13
  SPELL_ATTR_EX4_UNK14, // 14
  SPELL_ATTR_EX4_UNK15, // 15
  SPELL_ATTR_EX4_NOT_USABLE_IN_ARENA, // 16 not usable in arena
  SPELL_ATTR_EX4_USABLE_IN_ARENA, // 17 usable in arena
  SPELL_ATTR_EX4_UNK18, // 18
  SPELL_ATTR_EX4_UNK19, // 19
  SPELL_ATTR_EX4_UNK20, // 20 do not give "more powerful spell" error message
  SPELL_ATTR_EX4_UNK21, // 21
  SPELL_ATTR_EX4_UNK22, // 22
  SPELL_ATTR_EX4_UNK23, // 23
  SPELL_ATTR_EX4_UNK24, // 24
  SPELL_ATTR_EX4_UNK25, // 25 pet scaling auras
  SPELL_ATTR_EX4_CAST_ONLY_IN_OUTLAND, // 26 Can only be used in Outland.
  SPELL_ATTR_EX4_UNK27, // 27
  SPELL_ATTR_EX4_UNK28, // 28
  SPELL_ATTR_EX4_UNK29, // 29
  SPELL_ATTR_EX4_UNK30, // 30
  SPELL_ATTR_EX4_UNK31, // 31

  SPELL_ATTR_EX5_UNK0 = 0x0500, // 0
  SPELL_ATTR_EX5_NO_REAGENT_WHILE_PREP, // 1 not need reagents if UNIT_FLAG_PREPARATION
  SPELL_ATTR_EX5_UNK2, // 2 removed at enter arena (e.g. 31850 since 3.3.3)
  SPELL_ATTR_EX5_USABLE_WHILE_STUNNED, // 3 usable while stunned
  SPELL_ATTR_EX5_UNK4, // 4
  SPELL_ATTR_EX5_SINGLE_TARGET_SPELL, // 5 Only one target can be apply at a time
  SPELL_ATTR_EX5_UNK6, // 6
  SPELL_ATTR_EX5_UNK7, // 7
  SPELL_ATTR_EX5_UNK8, // 8
  SPELL_ATTR_EX5_START_PERIODIC_AT_APPLY, // 9 begin periodic tick at aura apply
  SPELL_ATTR_EX5_UNK10, // 10
  SPELL_ATTR_EX5_UNK11, // 11
  SPELL_ATTR_EX5_UNK12, // 12
  SPELL_ATTR_EX5_UNK13, // 13 haste affects duration (e.g. 8050 since 3.3.3)
  SPELL_ATTR_EX5_UNK14, // 14
  SPELL_ATTR_EX5_UNK15, // 15
  SPELL_ATTR_EX5_UNK16, // 16
  SPELL_ATTR_EX5_USABLE_WHILE_FEARED, // 17 usable while feared
  SPELL_ATTR_EX5_USABLE_WHILE_CONFUSED, // 18 usable while confused
  SPELL_ATTR_EX5_UNK19, // 19
  SPELL_ATTR_EX5_UNK20, // 20
  SPELL_ATTR_EX5_UNK21, // 21
  SPELL_ATTR_EX5_UNK22, // 22
  SPELL_ATTR_EX5_UNK23, // 23
  SPELL_ATTR_EX5_UNK24, // 24
  SPELL_ATTR_EX5_UNK25, // 25
  SPELL_ATTR_EX5_UNK26, // 26
  SPELL_ATTR_EX5_UNK27, // 27
  SPELL_ATTR_EX5_UNK28, // 28
  SPELL_ATTR_EX5_UNK29, // 29
  SPELL_ATTR_EX5_UNK30, // 30
  SPELL_ATTR_EX5_UNK31, // 31 Forces all nearby enemies to focus attacks caster

  SPELL_ATTR_EX6_UNK0 = 0x0600, // 0 Only Move spell have this flag
  SPELL_ATTR_EX6_ONLY_IN_ARENA, // 1 only usable in arena, not used in 3.2.0a and early
  SPELL_ATTR_EX6_UNK2, // 2
  SPELL_ATTR_EX6_UNK3, // 3
  SPELL_ATTR_EX6_UNK4, // 4
  SPELL_ATTR_EX6_UNK5, // 5
  SPELL_ATTR_EX6_UNK6, // 6
  SPELL_ATTR_EX6_UNK7, // 7
  SPELL_ATTR_EX6_UNK8, // 8
  SPELL_ATTR_EX6_UNK9, // 9
  SPELL_ATTR_EX6_UNK10, // 10
  SPELL_ATTR_EX6_NOT_IN_RAID_INSTANCE, // 11 not usable in raid instance
  SPELL_ATTR_EX6_UNK12, // 12 for auras AURA_TRACK_CREATURES, AURA_TRACK_RESOURCES and AURA_TRACK_STEALTHED select non-stacking tracking spells
  SPELL_ATTR_EX6_UNK13, // 13
  SPELL_ATTR_EX6_UNK14, // 14
  SPELL_ATTR_EX6_UNK15, // 15 not set in 3.0.3
  SPELL_ATTR_EX6_UNK16, // 16
  SPELL_ATTR_EX6_UNK17, // 17
  SPELL_ATTR_EX6_UNK18, // 18
  SPELL_ATTR_EX6_UNK19, // 19
  SPELL_ATTR_EX6_UNK20, // 20
  SPELL_ATTR_EX6_UNK21, // 21
  SPELL_ATTR_EX6_UNK22, // 22
  SPELL_ATTR_EX6_UNK23, // 23 not set in 3.0.3
  SPELL_ATTR_EX6_UNK24, // 24 not set in 3.0.3
  SPELL_ATTR_EX6_UNK25, // 25 not set in 3.0.3
  SPELL_ATTR_EX6_UNK26, // 26 not set in 3.0.3
  SPELL_ATTR_EX6_UNK27, // 27 not set in 3.0.3
  SPELL_ATTR_EX6_UNK28, // 28 not set in 3.0.3
  SPELL_ATTR_EX6_NO_DMG_PERCENT_MODS, // 29 do not apply damage percent mods (usually in cases where it has already been applied)
  SPELL_ATTR_EX6_UNK30, // 30 not set in 3.0.3
  SPELL_ATTR_EX6_UNK31, // 31 not set in 3.0.3

  SPELL_ATTR_EX7_UNK0 = 0x0700, // 0
  SPELL_ATTR_EX7_UNK1, // 1
  SPELL_ATTR_EX7_UNK2, // 2
  SPELL_ATTR_EX7_UNK3, // 3
  SPELL_ATTR_EX7_UNK4, // 4
  SPELL_ATTR_EX7_UNK5, // 5
  SPELL_ATTR_EX7_UNK6, // 6
  SPELL_ATTR_EX7_UNK7, // 7
  SPELL_ATTR_EX7_UNK8, // 8
  SPELL_ATTR_EX7_UNK9, // 9
  SPELL_ATTR_EX7_UNK10, // 10
  SPELL_ATTR_EX7_UNK11, // 11
  SPELL_ATTR_EX7_UNK12, // 12
  SPELL_ATTR_EX7_UNK13, // 13
  SPELL_ATTR_EX7_UNK14, // 14
  SPELL_ATTR_EX7_UNK15, // 15
  SPELL_ATTR_EX7_UNK16, // 16
  SPELL_ATTR_EX7_UNK17, // 17
  SPELL_ATTR_EX7_UNK18, // 18
  SPELL_ATTR_EX7_UNK19, // 19
  SPELL_ATTR_EX7_UNK20, // 20
  SPELL_ATTR_EX7_UNK21, // 21
  SPELL_ATTR_EX7_UNK22, // 22
  SPELL_ATTR_EX7_UNK23, // 23
  SPELL_ATTR_EX7_UNK24, // 24
  SPELL_ATTR_EX7_UNK25, // 25
  SPELL_ATTR_EX7_UNK26, // 26
  SPELL_ATTR_EX7_UNK27, // 27
  SPELL_ATTR_EX7_UNK28, // 28
  SPELL_ATTR_EX7_UNK29, // 29
  SPELL_ATTR_EX7_UNK30, // 30
  SPELL_ATTR_EX7_UNK31, // 31

  SPELL_ATTR_EX8_UNK0 = 0x0800, // 0
  SPELL_ATTR_EX8_UNK1, // 1
  SPELL_ATTR_EX8_UNK2, // 2
  SPELL_ATTR_EX8_UNK3, // 3
  SPELL_ATTR_EX8_UNK4, // 4
  SPELL_ATTR_EX8_UNK5, // 5
  SPELL_ATTR_EX8_UNK6, // 6
  SPELL_ATTR_EX8_UNK7, // 7
  SPELL_ATTR_EX8_UNK8, // 8
  SPELL_ATTR_EX8_UNK9, // 9
  SPELL_ATTR_EX8_UNK10, // 10
  SPELL_ATTR_EX8_UNK11, // 11
  SPELL_ATTR_EX8_UNK12, // 12
  SPELL_ATTR_EX8_UNK13, // 13
  SPELL_ATTR_EX8_UNK14, // 14
  SPELL_ATTR_EX8_UNK15, // 15
  SPELL_ATTR_EX8_UNK16, // 16
  SPELL_ATTR_EX8_UNK17, // 17
  SPELL_ATTR_EX8_UNK18, // 18
  SPELL_ATTR_EX8_UNK19, // 19
  SPELL_ATTR_EX8_UNK20, // 20
  SPELL_ATTR_EX8_UNK21, // 21
  SPELL_ATTR_EX8_UNK22, // 22
  SPELL_ATTR_EX8_UNK23, // 23
  SPELL_ATTR_EX8_UNK24, // 24
  SPELL_ATTR_EX8_UNK25, // 25
  SPELL_ATTR_EX8_UNK26, // 26
  SPELL_ATTR_EX8_UNK27, // 27
  SPELL_ATTR_EX8_UNK28, // 28
  SPELL_ATTR_EX8_UNK29, // 29
  SPELL_ATTR_EX8_UNK30, // 30
  SPELL_ATTR_EX8_UNK31, // 31

  SPELL_ATTR_EX9_UNK0 = 0x0900, // 0
  SPELL_ATTR_EX9_UNK1, // 1
  SPELL_ATTR_EX9_UNK2, // 2
  SPELL_ATTR_EX9_UNK3, // 3
  SPELL_ATTR_EX9_UNK4, // 4
  SPELL_ATTR_EX9_UNK5, // 5
  SPELL_ATTR_EX9_UNK6, // 6
  SPELL_ATTR_EX9_UNK7, // 7
  SPELL_ATTR_EX9_UNK8, // 8
  SPELL_ATTR_EX9_UNK9, // 9
  SPELL_ATTR_EX9_UNK10, // 10
  SPELL_ATTR_EX9_UNK11, // 11
  SPELL_ATTR_EX9_UNK12, // 12
  SPELL_ATTR_EX9_UNK13, // 13
  SPELL_ATTR_EX9_UNK14, // 14
  SPELL_ATTR_EX9_UNK15, // 15
  SPELL_ATTR_EX9_UNK16, // 16
  SPELL_ATTR_EX9_UNK17, // 17
  SPELL_ATTR_EX9_UNK18, // 18
  SPELL_ATTR_EX9_UNK19, // 19
  SPELL_ATTR_EX9_UNK20, // 20
  SPELL_ATTR_EX9_UNK21, // 21
  SPELL_ATTR_EX9_UNK22, // 22
  SPELL_ATTR_EX9_UNK23, // 23
  SPELL_ATTR_EX9_UNK24, // 24
  SPELL_ATTR_EX9_UNK25, // 25
  SPELL_ATTR_EX9_UNK26, // 26
  SPELL_ATTR_EX9_UNK27, // 27
  SPELL_ATTR_EX9_UNK28, // 28
  SPELL_ATTR_EX9_UNK29, // 29
  SPELL_ATTR_EX9_UNK30, // 30
  SPELL_ATTR_EX9_UNK31, // 31
};

// DBC related classes ======================================================


// SpellPower.dbc
struct spellpower_data_t
{
  unsigned _id;
  unsigned _spell_id;
  unsigned _aura_id;            // Spell id for the aura during which this power type is active
  int      _power_type_e;
  int      _cost;
  double   _cost_2;
  int      _cost_per_second;    // Unsure

  resource_type_e resource() const;
  unsigned id() const { return _id; }
  unsigned spell_id() const { return _spell_id; }
  unsigned aura_id() const { return _aura_id; }
  power_type_e type() const { return static_cast< power_type_e >( _power_type_e ); }
  double cost() const
  {
    double cost = 0.0;
    double divisor;

    if ( _cost > 0 )
      cost = _cost;
    else
      cost = _cost_2;

    switch ( type() )
    {
    case POWER_MANA:
      divisor = 100.0;
      break;
    case POWER_RAGE:
    case POWER_RUNIC_POWER:
      divisor = 10.0;
      break;
    default:
      divisor = 1.0;
      break;
    }

    return cost / divisor;
  }

  static spellpower_data_t* nil();
  static spellpower_data_t* list( bool ptr = false );
  static void               link( bool ptr = false );
};

class spellpower_data_nil_t : public spellpower_data_t
{
public:
  spellpower_data_nil_t();
  static spellpower_data_nil_t singleton;
};

inline spellpower_data_t* spellpower_data_t::nil()
{ return &spellpower_data_nil_t::singleton; }

// SpellEffect.dbc
struct spelleffect_data_t
{
private:
  static const unsigned int FLAG_USED = 1;
  static const unsigned int FLAG_ENABLED = 2;

public:
  unsigned         _id;              // Effect id
  unsigned         _flags;           // Unused for now, 0x00 for all
  unsigned         _spell_id;        // Spell this effect belongs to
  unsigned         _index;           // Effect index for the spell
  effect_type_t    _type;            // Effect type
  effect_subtype_t _subtype;         // Effect sub-type
  // SpellScaling.dbc
  double           _m_avg;           // Effect average spell scaling multiplier
  double           _m_delta;         // Effect delta spell scaling multiplier
  double           _m_unk;           // Unused effect scaling multiplier
  //
  double           _coeff;           // Effect coefficient
  double           _amplitude;       // Effect amplitude (e.g., tick time)
  // SpellRadius.dbc
  double           _radius;          // Minimum spell radius
  double           _radius_max;      // Maximum spell radius
  //
  int              _base_value;      // Effect value
  int              _misc_value;      // Effect miscellaneous value
  int              _misc_value_2;    // Effect miscellaneous value 2
  int              _trigger_spell_id;// Effect triggers this spell id
  double           _m_chain;         // Effect chain multiplier
  double           _pp_combo_points; // Effect points per combo points
  double           _real_ppl;        // Effect real points per level
  int              _die_sides;       // Effect damage range

  // Pointers for runtime linking
  spell_data_t*    _spell;
  spell_data_t*    _trigger_spell;

  bool                       ok() const { return _id != 0; }
  unsigned                   id() const { return _id; }
  unsigned                   index() const { return _index; }
  unsigned                   spell_id() const { return _spell_id; }
  unsigned                   spell_effect_num() const { return _index; }

  effect_type_t              type() const { return _type; }
  effect_subtype_t           subtype() const { return _subtype; }

  int                        base_value() const { return _base_value; }
  double                     percent() const { return _base_value * ( 1 / 100.0 ); }
  timespan_t                 time_value() const { return timespan_t::from_millis( _base_value ); }
  resource_type_e            resource_gain_type() const;

  double resource( resource_type_e resource_type ) const
  {
    switch ( resource_type )
    {
    case RESOURCE_RUNIC_POWER:
    case RESOURCE_RAGE:
      return _base_value * ( 1 / 10.0 );
    case RESOURCE_MANA:
      return _base_value * ( 1 / 100.0 );
    default:
      return _base_value;
    }
  }

  int                        misc_value1() const { return _misc_value; }
  int                        misc_value2() const { return _misc_value_2; }

  unsigned                   trigger_spell_id() const { return _trigger_spell_id >= 0 ? _trigger_spell_id : 0; }
  double                     chain_multiplier() const { return _m_chain; }

  double                     m_average() const { return _m_avg; }
  double                     m_delta() const { return _m_delta; }
  double                     m_unk() const { return _m_unk; }

  double                     coeff() const { return _coeff; }

  timespan_t                 period() const { return timespan_t::from_millis( _amplitude ); }

  double                     radius() const { return _radius; }
  double                     radius_max() const { return _radius_max; }

  double                     pp_combo_points() const { return _pp_combo_points; }

  double                     real_ppl() const { return _real_ppl; }

  int                        die_sides() const { return _die_sides; }

  double                     average( const player_t* p, unsigned level=0 ) const;
  double                     delta( const player_t* p, unsigned level=0 ) const;
  double                     bonus( const player_t* p, unsigned level=0 ) const;
  double                     min( const player_t* p, unsigned level=0 ) const;
  double                     max( const player_t* p, unsigned level=0 ) const;

  spell_data_t*              spell()   const;
  spell_data_t*              trigger() const;

  static spelleffect_data_t* nil();
  static spelleffect_data_t* find( unsigned, bool ptr = false );
  static spelleffect_data_t* list( bool ptr = false );
  static void                link( bool ptr = false );
};

class spelleffect_data_nil_t : public spelleffect_data_t
{
public:
  spelleffect_data_nil_t();
  static spelleffect_data_nil_t singleton;
};

inline spelleffect_data_t* spelleffect_data_t::nil()
{ return &spelleffect_data_nil_t::singleton; }

#ifdef __OpenBSD__
#pragma pack(1)
#else
#pragma pack( push, 1 )
#endif
struct spell_data_t
{
private:
  static const unsigned FLAG_USED = 1;
  static const unsigned FLAG_DISABLED = 2;

  friend class dbc_t;
  static void link( bool ptr );

public:
  const char* _name;               // Spell name from Spell.dbc stringblock (enGB)
  unsigned    _id;                 // Spell ID in dbc
  unsigned    _flags;              // Unused for now, 0x00 for all
  double      _prj_speed;          // Projectile Speed
  unsigned    _school;             // Spell school mask
  unsigned    _class_mask;         // Class mask for spell
  unsigned    _race_mask;          // Racial mask for the spell
  int         _scaling_type;       // Array index for gtSpellScaling.dbc. -1 means the last sub-array, 0 disabled
  double      _extra_coeff;        // An "extra" coefficient (used for some spells to indicate AP based coefficient)
  // SpellLevels.dbc
  unsigned    _spell_level;        // Spell learned on level. NOTE: Only accurate for "class abilities"
  unsigned    _max_level;          // Maximum level for scaling
  // SpellRange.dbc
  double      _min_range;          // Minimum range in yards
  double      _max_range;          // Maximum range in yards
  // SpellCooldown.dbc
  unsigned    _cooldown;           // Cooldown in milliseconds
  unsigned    _gcd;                // GCD in milliseconds
  // SpellCategories.dbc
  unsigned    _category;           // Spell category (for shared cooldowns, effects?)
  // SpellDuration.dbc
  double      _duration;           // Spell duration in milliseconds
  // SpellRuneCost.dbc
  unsigned    _rune_cost;          // Bitmask of rune cost 0x1, 0x2 = Blood | 0x4, 0x8 = Unholy | 0x10, 0x20 = Frost
  unsigned    _runic_power_gain;   // Amount of runic power gained ( / 10 )
  // SpellAuraOptions.dbc
  unsigned    _max_stack;          // Maximum stack size for spell
  unsigned    _proc_chance;        // Spell proc chance in percent
  unsigned    _proc_charges;       // Per proc charge amount
  // SpellEquippedItems.dbc
  unsigned    _equipped_class;
  unsigned    _equipped_invtype_mask;
  unsigned    _equipped_subclass_mask;
  // SpellScaling.dbc
  int         _cast_min;           // Minimum casting time in milliseconds
  int         _cast_max;           // Maximum casting time in milliseconds
  int         _cast_div;           // A divisor used in the formula for casting time scaling (20 always?)
  double      _c_scaling;          // A scaling multiplier for level based scaling
  unsigned    _c_scaling_level;    // A scaling divisor for level based scaling
  // SpecializationSpells.dbc
  unsigned    _replace_spell_id;
  // Spell.dbc flags
  unsigned    _attributes[NUM_SPELL_FLAGS];// Spell.dbc "flags", record field 1..10, note that 12694 added a field here after flags_7
  const char* _desc;               // Spell.dbc description stringblock
  const char* _tooltip;            // Spell.dbc tooltip stringblock
  // SpellDescriptionVariables.dbc
  const char* _desc_vars;          // Spell description variable stringblock, if present
  // SpellIcon.dbc
  const char* _icon;

  // Pointers for runtime linking
  std::vector<const spelleffect_data_t*>* _effects;
  std::vector<const spellpower_data_t*>*  _power;

  const spelleffect_data_t& effectN( unsigned idx ) const { assert( idx ); if ( idx > _effects -> size() ) return *spelleffect_data_t::nil(); else return *_effects -> at( idx - 1 ); }
  const spelleffect_data_t& effect1() const { return effectN( 1 ); }
  const spelleffect_data_t& effect2() const { return effectN( 2 ); }
  const spelleffect_data_t& effect3() const { return effectN( 3 ); }

  const spellpower_data_t& powerN( power_type_e pt ) const
  {
    assert( pt >= POWER_HEALTH && pt < POWER_MAX );
    for ( size_t i = 0; i < _power -> size(); i++ )
    {
      if ( _power -> at( i ) -> _power_type_e == pt )
        return *_power -> at( i );
    }

    return *spellpower_data_t::nil();
  }

  bool                 ok() const { return _id != 0; }

  unsigned             id() const { return _id; }
  uint32_t             school_mask() const { return _school; }
  static uint32_t      get_school_mask( school_type_e s );
  school_type_e        get_school_type() const;
  static bool          is_school( school_type_e s, school_type_e s2 );

  bool                 is_class( player_type_e c ) const;
  uint32_t             class_mask() const { return _class_mask; }

  bool                 is_race( race_type_e r ) const;
  uint32_t             race_mask() const { return _race_mask; }

  bool                 is_level( uint32_t level ) const { return level >= _spell_level; }
  uint32_t             level() const { return _spell_level; }
  uint32_t             max_level() const { return _max_level; }

  player_type_e        scaling_class() const;

  double               missile_speed() const { return _prj_speed; }
  double               min_range() const { return _min_range; }
  double               max_range() const { return _max_range; }
  bool                 in_range( double range ) const { return range >= _min_range && range <= _max_range; }

  timespan_t           cooldown() const { return timespan_t::from_millis( _cooldown ); }
  timespan_t           duration() const { return timespan_t::from_millis( _duration ); }
  timespan_t           gcd() const { return timespan_t::from_millis( _gcd ); }
  timespan_t           cast_time( uint32_t level ) const;
  double               cost( power_type_e ) const;

  uint32_t             category() const { return _category; }

  uint32_t             rune_cost() const { return _rune_cost; }
  double               runic_power_gain() const { return _runic_power_gain * ( 1 / 10.0 ); }

  uint32_t             max_stacks() const { return _max_stack; }
  uint32_t             initial_stacks() const { return _proc_charges; }

  double               proc_chance() const { return _proc_chance * ( 1 / 100.0 ); }

  uint32_t             effect_id( uint32_t effect_num ) const
  {
    assert( effect_num >= 1 && effect_num <= _effects -> size() );
    return _effects -> at( effect_num - 1 ) -> _id;
  }

  bool                 flags( spell_attribute_e f ) const;

  const char*          name_cstr() const { return ok() ? _name : ""; }
  const char*          desc() const { return ok() ? _desc : ""; }
  const char*          tooltip() const { return ok() ? _tooltip : ""; }

  double               scaling_multiplier() const { return _c_scaling; }
  unsigned             scaling_threshold() const { return _c_scaling_level; }
  double               extra_coeff() const { return _extra_coeff; }


  std::string          to_str() const;

  static spell_data_t* nil();
  static spell_data_t* not_found();
  static spell_data_t* find( const char* name, bool ptr = false );
  static spell_data_t* find( unsigned id, bool ptr = false );
  static spell_data_t* find( unsigned id, const char* confirmation, bool ptr = false );
  static spell_data_t* list( bool ptr = false );
} SC_PACKED_STRUCT;
#ifdef __OpenBSD__
#pragma pack()
#else
#pragma pack( pop )
#endif

class spell_data_nil_t : public spell_data_t
{
public:
  spell_data_nil_t();
  static spell_data_nil_t singleton;
};

inline spell_data_t* spell_data_t::nil()
{ return &spell_data_nil_t::singleton; }

class spell_data_not_found_t : public spell_data_t
{
public:
  spell_data_not_found_t();
  static spell_data_not_found_t singleton;
};

inline spell_data_t* spell_data_t::not_found()
{ return &spell_data_not_found_t::singleton; }


struct talent_data_t
{
public:
  const char * _name;        // Talent name
  unsigned     _id;          // Talent id
  unsigned     _flags;       // Unused for now, 0x00 for all
  unsigned     _m_class;     // Class mask
  unsigned     _m_pet;       // Pet mask
  unsigned     _is_pet;      // Is Pet?
  unsigned     _col;         // Talent column
  unsigned     _row;         // Talent row
  unsigned     _spell_id;    // Talent spell
  unsigned     _replace_id;  // Talent replaces the following spell id

  // Pointers for runtime linking
  spell_data_t* spell1;

  inline unsigned       id() const { return _id; }
  const char*           name_cstr() const { return _name; }
  bool                  is_class( player_type_e c ) const;
  bool                  is_pet( pet_type_e p ) const;
  unsigned              col() const { return _col; }
  unsigned              row() const { return _row; }
  unsigned              spell_id() const { return _spell_id; }
  unsigned              replace_id() const { return _replace_id; }
  unsigned              mask_class() const { return _m_class; }
  unsigned              mask_pet() const { return _m_pet; }

  spell_data_t* spell()
  {
    return spell1 ? spell1 : spell_data_t::nil();
  }

  static talent_data_t* nil();
  static talent_data_t* find( unsigned, bool ptr = false );
  static talent_data_t* find( unsigned, const char* confirmation, bool ptr = false );
  static talent_data_t* find( const char* name, bool ptr = false );
  static talent_data_t* find( player_type_e c, unsigned int row, unsigned int col, bool ptr = false );
  static talent_data_t* list( bool ptr = false );
  static void           link( bool ptr = false );
};

class talent_data_nil_t : public talent_data_t
{
public:
  talent_data_nil_t();
  static talent_data_nil_t singleton;
};

inline talent_data_t* talent_data_t::nil()
{ return &talent_data_nil_t::singleton; }

class dbc_t
{
public:
  bool ptr;

private:
  typedef std::unordered_map<uint32_t, uint32_t> id_map_t;
  id_map_t replaced_ids;
public:
  uint32_t replaced_id( uint32_t id_spell ) const;
  bool replace_id( uint32_t id_spell, uint32_t replaced_by_id );

  dbc_t( bool ptr=false ) : ptr( ptr ) { }

  // Static Initialization
  static void apply_hotfixes();
  static void init();
  static void de_init() {}

  static const char* build_level( bool ptr = false );
  static const char* wow_version( bool ptr = false );

  static const item_data_t* items( bool ptr = false );
  static std::size_t        n_items( bool ptr = false );

  // Game data table access
  double melee_crit_base( player_type_e t ) const;
  double melee_crit_base( pet_type_e t ) const;
  double spell_crit_base( player_type_e t ) const;
  double spell_crit_base( pet_type_e t ) const;
  double dodge_base( player_type_e t ) const;
  double dodge_base( pet_type_e t ) const;
  double regen_base( player_type_e t, unsigned level ) const;
  double regen_base( pet_type_e t, unsigned level ) const;
  double resource_base( player_type_e t, unsigned level ) const;
  double health_base( player_type_e t, unsigned level ) const;
  stat_data_t& attribute_base( player_type_e t, unsigned level ) const;
  stat_data_t& attribute_base( pet_type_e t, unsigned level ) const;
  stat_data_t& race_base( race_type_e r ) const;
  stat_data_t& race_base( pet_type_e t ) const;

  double spell_scaling( player_type_e t, unsigned level ) const;
  double melee_crit_scaling( player_type_e t, unsigned level ) const;
  double melee_crit_scaling( pet_type_e t, unsigned level ) const;
  double spell_crit_scaling( player_type_e t, unsigned level ) const;
  double spell_crit_scaling( pet_type_e t, unsigned level ) const;
  double dodge_scaling( player_type_e t, unsigned level ) const;
  double dodge_scaling( pet_type_e t, unsigned level ) const;
  double regen_spirit( player_type_e t, unsigned level ) const;
  double regen_spirit( pet_type_e t, unsigned level ) const;
  double mp5_per_spirit( player_type_e t, unsigned level ) const;
  double mp5_per_spirit( pet_type_e t, unsigned level ) const;
  double health_per_stamina( unsigned level ) const;

  double combat_rating( unsigned combat_rating_id, unsigned level ) const;
  double oct_combat_rating( unsigned combat_rating_id, player_type_e t ) const;

  const spell_data_t*            spell( unsigned spell_id ) const { return spell_data_t::find( spell_id, ptr ); }
  const spelleffect_data_t*      effect( unsigned effect_id ) const { return spelleffect_data_t::find( effect_id, ptr ); }
  const talent_data_t*           talent( unsigned talent_id ) const { return talent_data_t::find( talent_id, ptr ); }
  const item_data_t*             item( unsigned item_id ) const;

  const random_suffix_data_t&    random_suffix( unsigned suffix_id ) const;
  const item_enchantment_data_t& item_enchantment( unsigned enchant_id ) const;
  const gem_property_data_t&     gem_property( unsigned gem_id ) const;

  const random_prop_data_t&      random_property( unsigned ilevel ) const;
  const item_scale_data_t&       item_damage_1h( unsigned ilevel ) const;
  const item_scale_data_t&       item_damage_2h( unsigned ilevel ) const;
  const item_scale_data_t&       item_damage_caster_1h( unsigned ilevel ) const;
  const item_scale_data_t&       item_damage_caster_2h( unsigned ilevel ) const;
  const item_scale_data_t&       item_damage_ranged( unsigned ilevel ) const;
  const item_scale_data_t&       item_damage_thrown( unsigned ilevel ) const;
  const item_scale_data_t&       item_damage_wand( unsigned ilevel ) const;

  const item_scale_data_t&       item_armor_quality( unsigned ilevel ) const;
  const item_scale_data_t&       item_armor_shield( unsigned ilevel ) const;
  const item_armor_type_data_t&  item_armor_total( unsigned ilevel ) const;
  const item_armor_type_data_t&  item_armor_inv_type( unsigned inv_type ) const;

  // Derived data access
  unsigned class_ability( unsigned class_id, unsigned tree_id, unsigned n ) const;
  unsigned pet_ability( unsigned class_id, unsigned n ) const;
  unsigned class_ability_tree_size() const;
  unsigned class_ability_size() const;
  unsigned class_max_size() const;

  unsigned race_ability( unsigned race_id, unsigned class_id, unsigned n ) const;
  unsigned race_ability_size() const;
  unsigned race_ability_tree_size() const;

  unsigned specialization_ability( unsigned class_id, unsigned tree_id, unsigned n ) const;
  unsigned specialization_ability_size() const;
  unsigned specialization_max_per_class() const;
  unsigned specialization_max_class() const;
  specialization_e ability_specialization( uint32_t spell_id ) const;

  unsigned mastery_ability( unsigned class_id, unsigned tree_id, unsigned n ) const;
  unsigned mastery_ability_size() const;
  int      mastery_ability_tree( player_type_e c, uint32_t spell_id ) const;

  unsigned glyph_spell( unsigned class_id, unsigned glyph_type_e, unsigned n ) const;
  unsigned glyph_spell_size() const;

  unsigned set_bonus_spell( unsigned class_id, unsigned tier, unsigned n ) const;
  unsigned set_bonus_spell_size() const;

  // Helper methods
  double   weapon_dps( unsigned item_id ) const;

  double   effect_average( unsigned effect_id, unsigned level ) const;
  double   effect_delta( unsigned effect_id, unsigned level ) const;

  double   effect_min( unsigned effect_id, unsigned level ) const;
  double   effect_max( unsigned effect_id, unsigned level ) const;
  double   effect_bonus( unsigned effect_id, unsigned level ) const;

  unsigned talent_ability_id( player_type_e c, const char* spell_name ) const;
  unsigned class_ability_id( player_type_e c, specialization_e spec_id, const char* spell_name ) const;
  unsigned pet_ability_id( player_type_e c, const char* spell_name ) const;
  unsigned race_ability_id( player_type_e c, race_type_e r, const char* spell_name ) const;
  unsigned specialization_ability_id( specialization_e spec_id, const char* spell_name ) const;
  unsigned mastery_ability_id( specialization_e spec, const char* spell_name ) const;
  unsigned mastery_ability_id( specialization_e spec, uint32_t idx ) const;
  specialization_e mastery_specialization( const player_type_e c, uint32_t spell_id ) const;

  unsigned glyph_spell_id( player_type_e c, const char* spell_name ) const;
  unsigned set_bonus_spell_id( player_type_e c, const char* spell_name, int tier = -1 ) const;

  int      class_ability_tree( player_type_e c, uint32_t spell_id ) const;
  specialization_e class_ability_specialization( const player_type_e c, uint32_t spell_id ) const;

  bool     is_class_ability( uint32_t spell_id ) const;
  bool     is_race_ability( uint32_t spell_id ) const;
  bool     is_specialization_ability( uint32_t spell_id ) const;
  bool     is_mastery_ability( uint32_t spell_id ) const;
  bool     is_glyph_spell( uint32_t spell_id ) const;
  bool     is_set_bonus_spell( uint32_t spell_id ) const;

  bool spec_idx( specialization_e spec_id, uint32_t& class_idx, uint32_t& spec_index ) const;
  specialization_e spec_by_idx( const player_type_e c, uint32_t& idx ) const;

  // Static helper methods
  static double fmt_value( double v, effect_type_t type, effect_subtype_t sub_type );


  static const std::string& get_token( unsigned int id_spell );
  static bool add_token( unsigned int id_spell, const std::string& token_name, bool ptr = false );
};

// Options ==================================================================

enum option_type_e
{
  OPT_NONE=0,
  OPT_STRING,     // std::string*
  OPT_APPEND,     // std::string* (append)
  OPT_BOOL,       // int (only valid values are 1 and 0)
  OPT_INT,        // int
  OPT_FLT,        // double
  OPT_TIMESPAN,   // time interval
  OPT_LIST,       // std::vector<std::string>*
  OPT_FUNC,       // function pointer
  OPT_TALENT_RANK, // talent rank
  OPT_SPELL_ENABLED, // spell enabled
  OPT_DEPRECATED,
  OPT_UNKNOWN
};

typedef bool ( *option_function_t )( sim_t* sim, const std::string& name, const std::string& value );

struct option_t
{
  const char* name;
  option_type_e type;
  void* address;

  void print( FILE* );
  void save ( FILE* );
  bool parse( sim_t*, const std::string& name, const std::string& value );

  static void add( std::vector<option_t>&, const char* name, option_type_e type, void* address );
  static void copy( std::vector<option_t>& opt_vector, const option_t* opt_array );
  static bool parse( sim_t*, std::vector<option_t>&, const std::string& name, const std::string& value );
  static bool parse( sim_t*, const char* context, std::vector<option_t>&, const std::string& options_str );
  static bool parse( sim_t*, const char* context, std::vector<option_t>&, const std::vector<std::string>& strings );
  static bool parse( sim_t*, const char* context, const option_t*,        const std::vector<std::string>& strings );
  static bool parse( sim_t*, const char* context, const option_t*,        const std::string& options_str );
  static bool parse_file( sim_t*, FILE* file );
  static bool parse_line( sim_t*, const char* line );
  static bool parse_token( sim_t*, const std::string& token );
  static option_t* merge( std::vector<option_t>& out, const option_t* in1, const option_t* in2 );
};

// Talent Translation =======================================================

#ifndef MAX_TALENT_ROWS
#define MAX_TALENT_ROWS ( 6 )
#endif

#ifndef MAX_TALENT_COLS
#define MAX_TALENT_COLS ( 3 )
#endif

#ifndef MAX_TALENT_SLOTS
#define MAX_TALENT_SLOTS ( MAX_TALENT_ROWS * MAX_TALENT_COLS )
#endif


// Utilities ================================================================

#ifdef _MSC_VER
// C99-compliant snprintf - MSVC _snprintf is NOT the same.

#undef vsnprintf
int vsnprintf_simc( char* buf, size_t size, const char* fmt, va_list ap );
#define vsnprintf vsnprintf_simc

#undef snprintf
inline int snprintf( char* buf, size_t size, const char* fmt, ... )
{
  va_list ap;
  va_start( ap, fmt );
  int rval = vsnprintf( buf, size, fmt, ap );
  va_end( ap );
  return rval;
}
#endif

struct util_t
{
private:
  static void str_to_utf8_( std::string& str );
  static void str_to_latin1_( std::string& str );
  static void urlencode_( std::string& str );
  static void urldecode_( std::string& str );
  static void format_text_( std::string& name, bool input_is_utf8 );
  static void html_special_char_decode_( std::string& str );
  static void tolower_( std::string& );
  static void string_split_( std::vector<std::string>& results, const std::string& str, const char* delim, bool allow_quotes );
  static void replace_all_( std::string&, const char* from, char to );
  static void replace_all_( std::string&, char from, const char* to );
  static int vfprintf_helper( FILE *stream, const char *format, va_list fmtargs );

public:
  static double talent_rank( int num, int max, double increment );
  static double talent_rank( int num, int max, double value1, double value2, ... );

  static int talent_rank( int num, int max, int increment );
  static int talent_rank( int num, int max, int value1, int value2, ... );

  static double ability_rank( int player_level, double ability_value, int ability_level, ... );
  static int    ability_rank( int player_level, int    ability_value, int ability_level, ... );

  static const char* attribute_type_string     ( attribute_type_e type );
  static const char* dmg_type_string           ( dmg_type_e type );
  static const char* dot_behaviour_type_string ( dot_behaviour_type_e t );
  static const char* elixir_type_string        ( elixir_type_e type );
  static const char* flask_type_string         ( flask_type_e type );
  static const char* food_type_string          ( food_type_e type );
  static const char* gem_type_string           ( gem_type_e type );
  static const char* meta_gem_type_string      ( meta_gem_type_e type );
  static const char* player_type_string        ( player_type_e );
  static const char* pet_type_string           ( pet_type_e type );
  static const char* position_type_string      ( position_type_e );
  static const char* profession_type_string    ( profession_type_e );
  static const char* race_type_string          ( race_type_e );
  static const char* role_type_string          ( role_type_e );
  static const char* resource_type_string      ( resource_type_e );
  static const char* result_type_string        ( result_type_e type );
  static uint32_t    school_type_component     ( school_type_e s_type, school_type_e c_type );
  static const char* school_type_string        ( school_type_e type );
  static const char* armor_type_string         ( player_type_e ptype, slot_type_e );
  static const char* set_bonus_string          ( set_type_e type );

  static const char* slot_type_string          ( slot_type_e type );
  static const char* stat_type_string          ( stat_type_e type );
  static const char* stat_type_abbrev          ( stat_type_e type );
  static const char* stat_type_wowhead         ( stat_type_e type );
  static specialization_e translate_spec_str   ( player_type_e ptype, const std::string& spec_str );
  static const char* specialization_string     ( specialization_e spec, bool armory_format = true );
  static resource_type_e translate_power_type  ( power_type_e );
  static const char* weapon_type_string        ( weapon_type_e type );
  static const char* weapon_class_string       ( inventory_type class_ );
  static const char* weapon_subclass_string    ( item_subclass_weapon subclass );

  static const char* set_item_type_string      ( int item_set );
  static const char* item_quality_string       ( int item_quality );

  static attribute_type_e parse_attribute_type ( const std::string& name );
  static dmg_type_e parse_dmg_type             ( const std::string& name );
  static elixir_type_e parse_elixir_type       ( const std::string& name );
  static flask_type_e parse_flask_type         ( const std::string& name );
  static food_type_e parse_food_type           ( const std::string& name );
  static gem_type_e parse_gem_type             ( const std::string& name );
  static meta_gem_type_e parse_meta_gem_type   ( const std::string& name );
  static player_type_e parse_player_type       ( const std::string& name );
  static pet_type_e parse_pet_type             ( const std::string& name );
  static profession_type_e parse_profession_type( const std::string& name );
  static position_type_e parse_position_type   ( const std::string& name );
  static race_type_e parse_race_type           ( const std::string& name );
  static role_type_e parse_role_type           ( const std::string& name );
  static resource_type_e parse_resource_type   ( const std::string& name );
  static result_type_e parse_result_type       ( const std::string& name );
  static school_type_e parse_school_type       ( const std::string& name );
  static set_type_e parse_set_bonus            ( const std::string& name );
  static slot_type_e parse_slot_type           ( const std::string& name );
  static stat_type_e parse_stat_type           ( const std::string& name );
  static stat_type_e parse_reforge_type        ( const std::string& name );

  static weapon_type_e parse_weapon_type       ( const std::string& name );

  static int parse_item_quality                ( const std::string& quality );

  static bool parse_origin( std::string& region, std::string& server, std::string& name, const std::string& origin );

  static int class_id_mask( player_type_e type );
  static int class_id( player_type_e type );
  static unsigned race_mask( race_type_e type );
  static unsigned race_id( race_type_e type );
  static unsigned pet_mask( pet_type_e type );
  static unsigned pet_id( pet_type_e type );
  static player_type_e pet_class_type( pet_type_e type );

  static const char* class_id_string( player_type_e type );
  static player_type_e translate_class_id( int cid );
  static player_type_e translate_class_str( std::string& s );
  static race_type_e translate_race_id( int rid );
  static stat_type_e translate_item_mod( item_mod_type stat_mod );
  static slot_type_e translate_invtype( inventory_type inv_type );
  static weapon_type_e translate_weapon_subclass( item_subclass_weapon weapon_subclass );
  static profession_type_e translate_profession_id( int skill_id );
  static gem_type_e translate_socket_color( item_socket_color );

  static bool socket_gem_match( gem_type_e socket, gem_type_e gem );

  static size_t string_split( std::vector<std::string>& results, const std::string& str, const char* delim, bool allow_quotes = false )
  { string_split_( results, str, delim, allow_quotes ); return results.size(); }
  static int string_split( const std::string& str, const char* delim, const char* format, ... );
  static void string_strip_quotes( std::string& str );
  static std::string& replace_all( std::string& s, const char* from, char to )
  { replace_all_( s, from, to ); return s; }
  static std::string& replace_all( std::string& s, char from, const char* to )
  { replace_all_( s, from, to ); return s; }

  template <typename T>
  static std::string to_string( const T& t )
  { std::ostringstream s; s << t; return s.str(); }

  static std::string to_string( double f );
  static std::string to_string( double f, int precision );

  static int64_t milliseconds();
  static int64_t parse_date( const std::string& month_day_year );

  static int printf( const char *format, ... ) PRINTF_ATTRIBUTE( 1,2 );
  static int fprintf( FILE *stream, const char *format, ... ) PRINTF_ATTRIBUTE( 2,3 );
  static int vfprintf( FILE *stream, const char *format, va_list fmtargs ) PRINTF_ATTRIBUTE( 2,0 )
  { return vfprintf_helper( stream, format, fmtargs ); }
  static int vprintf( const char *format, va_list fmtargs ) PRINTF_ATTRIBUTE( 1,0 )
  { return vfprintf( stdout, format, fmtargs ); }

  static std::string& str_to_utf8( std::string& str ) { str_to_utf8_( str ); return str; }
  static std::string& str_to_latin1( std::string& str ) { str_to_latin1_( str ); return str; }
  static std::string& urlencode( std::string& str ) { urlencode_( str ); return str; }
  static std::string& urldecode( std::string& str ) { urldecode_( str ); return str; }

  static std::string& format_text( std::string& name, bool input_is_utf8 )
  { format_text_( name, input_is_utf8 ); return name; }

  static std::string& html_special_char_decode( std::string& str )
  { html_special_char_decode_( str ); return str; }

  static bool str_compare_ci( const std::string& l, const std::string& r );
  static std::string& glyph_name( std::string& n );
  static bool str_in_str_ci ( const std::string& l, const std::string& r );
  static bool str_prefix_ci ( const std::string& str, const std::string& prefix );

  static double floor( double X, unsigned int decplaces = 0 );
  static double ceil( double X, unsigned int decplaces = 0 );
  static double round( double X, unsigned int decplaces = 0 );

  static std::string& tolower( std::string& str ) { tolower_( str ); return str; }
  static std::string encode_html( const std::string& );

  static int snprintf( char* buf, size_t size, const char* fmt, ... ) PRINTF_ATTRIBUTE( 3,4 );
};

// Spell information struct, holding static functions to output spell data in a human readable form

struct spell_info_t
{
  static std::string to_str( sim_t* sim, const spell_data_t* spell, int level=MAX_LEVEL );
  //static std::string to_str( sim_t* sim, uint32_t spell_id, int level=MAX_LEVEL );
  static std::string talent_to_str( sim_t* sim, const talent_data_t* talent, int level=MAX_LEVEL );
  static std::ostringstream& effect_to_str( sim_t* sim, const spell_data_t* spell, const spelleffect_data_t* effect, std::ostringstream& s, int level=MAX_LEVEL );
};

// Spell ID class

enum s_type_e
{
  T_SPELL = 0,
  T_TALENT,
  T_MASTERY,
  T_GLYPH,
  T_CLASS,
  T_RACE,
  T_SPEC,
  T_ITEM
};


// Raid Event

struct raid_event_t
{
  sim_t* const sim;
  std::string name_str;
  int64_t num_starts;
  timespan_t first, last;
  timespan_t cooldown;
  timespan_t cooldown_stddev;
  timespan_t cooldown_min;
  timespan_t cooldown_max;
  timespan_t duration;
  timespan_t duration_stddev;
  timespan_t duration_min;
  timespan_t duration_max;
  double     distance_min;
  double     distance_max;
  timespan_t saved_duration;
  rng_t* rng;
  std::vector<player_t*> affected_players;

  raid_event_t( sim_t*, const std::string& );
  virtual ~raid_event_t() {}

  virtual timespan_t cooldown_time() const;
  virtual timespan_t duration_time() const;
  virtual void schedule();
  virtual void reset();
  virtual void start();
  virtual void finish();
  virtual void parse_options( option_t*, const std::string& options_str );
  static raid_event_t* create( sim_t* sim, const std::string& name, const std::string& options_str );
  static void init( sim_t* );
  static void reset( sim_t* );
  static void combat_begin( sim_t* );
  static void combat_end( sim_t* ) {}
};

// Gear Stats ===============================================================

namespace internal {
struct gear_stats_t
{
  std::array<double,ATTRIBUTE_MAX> attribute;
  std::array<double,RESOURCE_MAX> resource;
  double spell_power;
  double mp5;
  double attack_power;
  double expertise_rating;
  double expertise_rating2;
  double hit_rating;
  double hit_rating2;
  double crit_rating;
  double haste_rating;
  double weapon_dps;
  double weapon_speed;
  double weapon_offhand_dps;
  double weapon_offhand_speed;
  double armor;
  double bonus_armor;
  double dodge_rating;
  double parry_rating;
  double block_rating;
  double mastery_rating;
};
}

struct gear_stats_t : public internal::gear_stats_t
{
  typedef internal::gear_stats_t base_t;
  gear_stats_t() : base_t( base_t() ) {}

  void   add_stat( stat_type_e stat, double value );
  void   set_stat( stat_type_e stat, double value );
  double get_stat( stat_type_e stat ) const;
  void   print( FILE* );
  static double stat_mod( stat_type_e stat );
};

// Statistical Sample Data

struct sample_data_t
{
  // Analyzed Results
  double sum;
  double mean;
  double min;
  double max;
  double variance;
  double std_dev;
  double mean_std_dev;
  std::vector<int> distribution;
  const bool simple;
  const bool min_max;
private:
  std::vector<double> _data;
  int count;

  bool analyzed_basics;
  bool analyzed_variance;
  bool created_dist;
  bool is_sorted;
public:
  sample_data_t( bool s=true, bool mm=false );

  void reserve( std::size_t capacity )
  { if ( ! simple ) _data.reserve( capacity ); }
  void add( double x=0 );

  bool basics_analyzed() const { return analyzed_basics; }
  bool variance_analyzed() const { return analyzed_variance; }
  bool distribution_created() const { return created_dist; }
  bool sorted() const { return is_sorted; }
  int size() const { if ( simple ) return count; return ( int ) _data.size(); }

  void analyze(
    bool calc_basics=true,
    bool calc_variance=true,
    bool s=true,
    unsigned int create_dist=0 );
  void analyze_basics();
  void analyze_variance();
  void sort();
  void create_distribution( unsigned int num_buckets=50 );
  void clear() { count = 0; sum = 0; _data.clear(); distribution.clear(); }

  // Access functions
  double percentile( double ) const;
  const std::vector<double>& data() const { return _data; }
  void merge( const sample_data_t& );

  static double pearson_correlation( const sample_data_t&, const sample_data_t& );
};

// Buffs ====================================================================

struct buff_creator_t
{
private:
  actor_pair_t _player;
  sim_t* const _sim;
  std::string _name;
  const spell_data_t* s_data;
  double _chance;
  double _default_value;
  int _max_stack;
  timespan_t _duration, _cooldown;
  int _quiet, _reverse, _activated;
  friend struct buff_t;
  friend struct debuff_t;
private:
  void init();
public:
  buff_creator_t( actor_pair_t, const std::string& name, const spell_data_t* = spell_data_t::nil() );
  buff_creator_t( actor_pair_t, uint32_t id, const std::string& name );
  buff_creator_t( sim_t*, const std::string& name, const spell_data_t* = spell_data_t::nil() );

  buff_creator_t& duration( timespan_t d )
  { _duration=d; return *this; }
  buff_creator_t& default_value( double v )
  { _default_value=v; return *this; }
  buff_creator_t& chance( double c )
  { _chance=c; return *this; }
  buff_creator_t& max_stack( unsigned ms )
  { _max_stack=ms; return *this; }
  buff_creator_t& cd( timespan_t t )
  { _cooldown=t; return *this; }
  buff_creator_t& reverse( bool r )
  { _reverse=r; return *this; }
  buff_creator_t& quiet( bool q )
  { _quiet=q; return *this; }
  buff_creator_t& activated( bool a )
  { _activated=a; return *this; }
  buff_creator_t& spell( const spell_data_t* s )
  { s_data=s; return *this; }

  operator buff_t* () const;
  operator debuff_t* () const;
};

struct stat_buff_creator_t
{
private:
  buff_creator_t bc;
  double _amount;
  stat_type_e _stat;
  friend struct stat_buff_t;
public:
  stat_buff_creator_t( buff_creator_t a ) :
    bc( a ), _amount( 0 ), _stat( STAT_NONE ) {}

  stat_buff_creator_t& amount( double a )
  { _amount=a; return *this; }
  stat_buff_creator_t& stat( stat_type_e s )
  { _stat=s; return *this; }

  operator stat_buff_t* () const;
};

struct cost_reduction_buff_creator_t
{
private:
  buff_creator_t bc;
  double _amount;
  school_type_e _school;
  bool _refreshes;
  friend struct cost_reduction_buff_t;
public:
  cost_reduction_buff_creator_t( buff_creator_t a ) :
    bc( a ), _amount( 0 ), _school( SCHOOL_NONE ), _refreshes( false )
  {}

  cost_reduction_buff_creator_t& amount( double a )
  { _amount=a; return *this; }
  cost_reduction_buff_creator_t& school( school_type_e s )
  { _school=s; return *this; }
  cost_reduction_buff_creator_t& refreshes( bool r )
  { _refreshes=r; return *this; }

  operator cost_reduction_buff_t* () const;
};

struct buff_t
{
  sim_t* const sim;
  player_t* const player;
  std::string name_str;
  const spell_data_t* s_data;
  double default_value;
  double current_value, react;
  timespan_t buff_duration, buff_cooldown;
  double default_chance;
  timespan_t last_start;
  timespan_t last_trigger;
  timespan_t iteration_uptime_sum;
  int64_t up_count, down_count, start_count, refresh_count;
  int64_t trigger_attempts, trigger_successes;
  double benefit_pct, trigger_pct, avg_start, avg_refresh;
  std::vector<timespan_t> stack_occurrence, stack_react_time;
  std::vector<buff_uptime_t*> stack_uptime;
  player_t* source;
  player_t* initial_source;
  event_t* expiration;
  event_t* delay;
  rng_t* rng;
  cooldown_t* cooldown;
  int current_stack, max_stack;
  int aura_id;
  bool activated;
  bool reverse, constant, quiet, overridden;
  sample_data_t uptime_pct;
  sample_data_t start_intervals;
  sample_data_t trigger_intervals;

  virtual ~buff_t();

protected:
  buff_t( const buff_creator_t& params );
  friend struct buff_creator_t;
public:

  // Use check() inside of ready() methods to prevent skewing of "benefit" calculations.
  // Use up() where the presence of the buff affects the action mechanics.

  const spell_data_t& data() const { return *s_data; }
  int    check() const { return current_stack; }
  inline bool   up()    { if ( current_stack > 0 ) { up_count++; } else { down_count++; } return current_stack > 0; }
  inline int    stack() { if ( current_stack > 0 ) { up_count++; } else { down_count++; } return current_stack; }
  inline double value() { if ( current_stack > 0 ) { up_count++; } else { down_count++; } return current_value; }
  timespan_t remains() const;
  bool   remains_gt( timespan_t time ) const;
  bool   remains_lt( timespan_t time ) const;
  bool   trigger  ( action_t*, int stacks=1, double value=-1.0, timespan_t duration = timespan_t::min() );
  virtual bool   trigger  ( int stacks=1, double value=-1.0, double chance=-1.0, timespan_t duration = timespan_t::min() );
  virtual void   execute ( int stacks=1, double value=-1.0, timespan_t duration = timespan_t::min() );
  virtual void   increment( int stacks=1, double value=-1.0, timespan_t duration = timespan_t::min() );
  void   decrement( int stacks=1, double value=-1.0 );
  void   extend_duration( player_t* p, timespan_t seconds );

  virtual void start    ( int stacks=1, double value=-1.0, timespan_t duration = timespan_t::min() );
  virtual void refresh  ( int stacks=0, double value=-1.0, timespan_t duration = timespan_t::min() );
  virtual void bump     ( int stacks=1, double value=-1.0 );
  virtual void override ( int stacks=1, double value=-1.0 );
  virtual bool may_react( int stacks=1 ) const;
  virtual int stack_react() const;
  virtual void expire();
  virtual void predict();
  virtual void reset();
  virtual void aura_gain();
  virtual void aura_loss();
  virtual void merge( const buff_t* other_buff );
  virtual void analyze();
  void init_buff_shared();
  void init();
  virtual void combat_begin();
  virtual void combat_end();

  expr_t* create_expression( const std::string& type );
  std::string to_str() const;

  static buff_t* find( const std::vector<buff_t*>&, const std::string& name );
  static buff_t* find(    sim_t*, const std::string& name );
  static buff_t* find( player_t*, const std::string& name );

  const spelleffect_data_t& effect1() const { return s_data -> effect1(); }
  const spelleffect_data_t& effect2() const { return s_data -> effect2(); }
  const spelleffect_data_t& effect3() const { return s_data -> effect3(); }
};

inline buff_creator_t::operator buff_t* () const
{ return new buff_t( *this ); }

struct stat_buff_t : public buff_t
{
  double amount;
  stat_type_e stat;

  virtual void bump     ( int stacks=1, double value=-1.0 );
  virtual void decrement( int stacks=1, double value=-1.0 );
  virtual void expire();

private:
  stat_buff_t( const stat_buff_creator_t& params );
  friend struct stat_buff_creator_t;
};

inline stat_buff_creator_t::operator stat_buff_t* () const
{ return new stat_buff_t( *this ); }

struct cost_reduction_buff_t : public buff_t
{
  double amount;
  school_type_e school;
  bool refreshes;

private:
  cost_reduction_buff_t( const cost_reduction_buff_creator_t& params );
  friend struct cost_reduction_buff_creator_t;
public:
  virtual void bump     ( int stacks=1, double value=-1.0 );
  virtual void decrement( int stacks=1, double value=-1.0 );
  virtual void expire();
  virtual void refresh  ( int stacks=0, double value=-1.0, timespan_t duration = timespan_t::min() );
};

inline cost_reduction_buff_creator_t::operator cost_reduction_buff_t* () const
{ return new cost_reduction_buff_t( *this ); }

struct debuff_t : public buff_t
{
private:
  debuff_t( const buff_creator_t& params );
  friend struct buff_creator_t;
};

inline buff_creator_t::operator debuff_t* () const
{ return new debuff_t( *this ); }

typedef struct buff_t aura_t;

// Expressions ==============================================================

enum token_type_e
{
  TOK_UNKNOWN=0,
  TOK_PLUS,
  TOK_MINUS,
  TOK_MULT,
  TOK_DIV,
  TOK_ADD,
  TOK_SUB,
  TOK_AND,
  TOK_OR,
  TOK_NOT,
  TOK_EQ,
  TOK_NOTEQ,
  TOK_LT,
  TOK_LTEQ,
  TOK_GT,
  TOK_GTEQ,
  TOK_LPAR,
  TOK_RPAR,
  TOK_IN,
  TOK_NOTIN,
  TOK_NUM,
  TOK_STR,
  TOK_ABS,
  TOK_SPELL_LIST,
  TOK_FLOOR,
  TOK_CEIL
};

struct expr_token_t
{
  token_type_e type;
  std::string label;
};

struct expression_t
{
  static int precedence( token_type_e );
  static bool is_unary( token_type_e );
  static bool is_binary( token_type_e );
  static token_type_e next_token( action_t* action, const std::string& expr_str, int& current_index,
                                  std::string& token_str, token_type_e prev_token );
  static std::vector<expr_token_t> parse_tokens( action_t* action, const std::string& expr_str );
  static void print_tokens( const std::vector<expr_token_t>& tokens, sim_t* sim );
  static void convert_to_unary( std::vector<expr_token_t>& tokens );
  static bool convert_to_rpn( std::vector<expr_token_t>& tokens );
};

// Action expression types ==================================================

class expr_t
{
  std::string name_;

protected:
  template <typename T> static double coerce( T t ) { return t; }
  static double coerce( timespan_t t ) { return t.total_seconds(); }

  virtual double evaluate() = 0;

public:
  expr_t( const std::string& name ) : name_( name ) {}
  virtual ~expr_t() {}

  const std::string& name() const { return name_; }

  double eval() { return evaluate(); }
  bool success() { return eval() != 0; }

  static expr_t* parse( action_t*, const std::string& expr_str );
};

// Reference Expression - ref_expr_t
// Class Template to create a expression with a reference ( ref ) of arbitrary type T, and evaluate that reference
template <typename T>
class ref_expr_t : public expr_t
{
  const T& t;
  virtual double evaluate() { return coerce( t ); }

public:
  ref_expr_t( const std::string& name, const T& t_ ) : expr_t( name ), t( t_ ) {}
};

// Template to return a reference expression
template <typename T>
inline expr_t* make_ref_expr( const std::string& name, const T& t )
{ return new ref_expr_t<T>( name, t ); }

// Function Expression - fn_expr_t
// Class Template to create a function ( fn ) expression with arbitrary functor f, which gets evaluated
template <typename F>
class fn_expr_t : public expr_t
{
  F f;

  virtual double evaluate() { return coerce( f() ); }

public:
  fn_expr_t( const std::string& name, F f_ ) :
    expr_t( name ), f( f_ ) {}
};

// Template to return a function expression
template <typename F>
inline expr_t* make_fn_expr( const std::string& name, F f )
{ return new fn_expr_t<F>( name, f ); }

// Make member function expression - make_mem_fn_expr
// Template to return function expression that calls a member ( mem ) function ( fn ) f on reference t.
template <typename F, typename T>
inline expr_t* make_mem_fn_expr( const std::string& name, T& t, F f )
{ return make_fn_expr( name, std::bind( std::mem_fn( f ), &t ) ); }


// Spell query expression types =============================================

enum expr_data_type_e
{
  DATA_SPELL = 0,
  DATA_TALENT,
  DATA_EFFECT,
  DATA_TALENT_SPELL,
  DATA_CLASS_SPELL,
  DATA_RACIAL_SPELL,
  DATA_MASTERY_SPELL,
  DATA_SPECIALIZATION_SPELL,
  DATA_GLYPH_SPELL,
  DATA_SET_BONUS_SPELL
};

struct spell_data_expr_t
{
  std::string name_str;
  sim_t* sim;
  expr_data_type_e data_type;
  bool effect_query;

  int result_type_e;
  double result_num;
  std::vector<uint32_t> result_spell_list;
  std::string result_str;

  spell_data_expr_t( sim_t* sim, const std::string& n, expr_data_type_e dt = DATA_SPELL, bool eq = false, int t=TOK_UNKNOWN ) : name_str( n ), sim( sim ), data_type( dt ), effect_query( eq ), result_type_e( t ), result_num( 0 ), result_spell_list() {}
  spell_data_expr_t( sim_t* sim, const std::string& n, double       constant_value ) : name_str( n ), sim( sim ), data_type( DATA_SPELL ) { result_type_e = TOK_NUM; result_num = constant_value; }
  spell_data_expr_t( sim_t* sim, const std::string& n, std::string& constant_value ) : name_str( n ), sim( sim ), data_type( DATA_SPELL ) { result_type_e = TOK_STR; result_str = constant_value; }
  spell_data_expr_t( sim_t* sim, const std::string& n, std::vector<uint32_t>& constant_value ) : name_str( n ), sim( sim ), data_type( DATA_SPELL ) { result_type_e = TOK_SPELL_LIST; result_spell_list = constant_value; }
  virtual ~spell_data_expr_t() {}
  virtual int evaluate() { return result_type_e; }
  virtual const char* name() { return name_str.c_str(); }

  virtual std::vector<uint32_t> operator|( const spell_data_expr_t& /* other */ ) { return std::vector<uint32_t>(); }
  virtual std::vector<uint32_t> operator&( const spell_data_expr_t& /* other */ ) { return std::vector<uint32_t>(); }
  virtual std::vector<uint32_t> operator-( const spell_data_expr_t& /* other */ ) { return std::vector<uint32_t>(); }

  virtual std::vector<uint32_t> operator<( const spell_data_expr_t& /* other */ ) { return std::vector<uint32_t>(); }
  virtual std::vector<uint32_t> operator>( const spell_data_expr_t& /* other */ ) { return std::vector<uint32_t>(); }
  virtual std::vector<uint32_t> operator<=( const spell_data_expr_t& /* other */ ) { return std::vector<uint32_t>(); }
  virtual std::vector<uint32_t> operator>=( const spell_data_expr_t& /* other */ ) { return std::vector<uint32_t>(); }
  virtual std::vector<uint32_t> operator==( const spell_data_expr_t& /* other */ ) { return std::vector<uint32_t>(); }
  virtual std::vector<uint32_t> operator!=( const spell_data_expr_t& /* other */ ) { return std::vector<uint32_t>(); }

  virtual std::vector<uint32_t> in( const spell_data_expr_t& /* other */ ) { return std::vector<uint32_t>(); }
  virtual std::vector<uint32_t> not_in( const spell_data_expr_t& /* other */ ) { return std::vector<uint32_t>(); }

  static spell_data_expr_t* parse( sim_t* sim, const std::string& expr_str );
  static spell_data_expr_t* create_spell_expression( sim_t* sim, const std::string& name_str );
};

class mutex_t : public noncopyable
{
private:
  class native_t;
  native_t* native_handle;

public:
  mutex_t();
  ~mutex_t();

  void lock();
  void unlock();
};

class thread_t : public noncopyable
{
private:
  class native_t;
  native_t* native_handle;

protected:
  thread_t();
  virtual ~thread_t();
public:
  virtual void run() = 0;

  void launch();
  void wait();

  static void sleep( timespan_t );
};

class auto_lock_t
{
private:
  mutex_t& mutex;
public:
  auto_lock_t( mutex_t& mutex_ ) : mutex( mutex_ ) { mutex.lock(); }
  ~auto_lock_t() { mutex.unlock(); }
};

// Simple freelist allocator for events =====================================

class event_freelist_t
{
private:
  struct free_event_t { free_event_t* next; };
  free_event_t* list;

public:
  event_freelist_t() : list( 0 ) {}
  ~event_freelist_t();

  void* allocate( std::size_t );
  void deallocate( void* );
};

// Simulation Engine ========================================================

#define REGISTER_DOT(n) sim->register_targetdata_item(DATA_DOT, #n, t, nonpod_offsetof(type, dots_##n))
#define REGISTER_BUFF(n) sim->register_targetdata_item(DATA_AURA, #n, t, nonpod_offsetof(type, buffs_##n))
#define REGISTER_DEBUFF(n) sim->register_targetdata_item(DATA_AURA, #n, t, nonpod_offsetof(type, debuffs_##n))

struct sim_t : private thread_t
{
  int         argc;
  char**      argv;
  sim_t*      parent;
  event_freelist_t free_list;
  player_t*   target;
  player_t*   target_list;
  player_t*   player_list;
  player_t*   active_player;
  int         num_players;
  int         num_enemies;
  int         num_targetdata_ids;
  int         max_player_level;
  int         canceled;
  timespan_t  queue_lag, queue_lag_stddev;
  timespan_t  gcd_lag, gcd_lag_stddev;
  timespan_t  channel_lag, channel_lag_stddev;
  timespan_t  queue_gcd_reduction;
  int         strict_gcd_queue;
  double      confidence;
  double      confidence_estimator;
  // Latency
  timespan_t  world_lag, world_lag_stddev;
  double      travel_variance, default_skill;
  timespan_t  reaction_time, regen_periodicity;
  timespan_t  current_time, max_time, expected_time;
  double      vary_combat_length;
  timespan_t  last_event;
  int         fixed_time;
  int64_t     events_remaining, max_events_remaining;
  int64_t     events_processed, total_events_processed;
  int         seed, id, iterations, current_iteration, current_slot;
  int         armor_update_interval, weapon_speed_scale_factors;
  int         optimal_raid, log, debug;
  int         save_profiles, default_actions;
  stat_type_e normalized_stat;
  std::string current_name, default_region_str, default_server_str, save_prefix_str,save_suffix_str;
  int         save_talent_str;
  bool        input_is_utf8;
  std::vector<player_t*> actor_list;
  std::string main_target_str;
  double      dtr_proc_chance;

  // Target options
  double      target_death_pct;
  int         target_level;
  std::string target_race;
  int         target_adds;

  // Data access
  dbc_t       dbc;

  // Default stat enchants
  gear_stats_t enchant;

  std::unordered_map<std::string,std::string> var_map;
  std::vector<option_t> options;
  std::vector<std::string> party_encoding;
  std::vector<std::string> item_db_sources;

  // Random Number Generation
private:
  rng_t* default_rng_;     // == (deterministic_rng ? deterministic_rng : rng )
  rng_t* rng_list;
  int deterministic_rng;
public:
  rng_t* rng;
  rng_t* _deterministic_rng;
  int separated_rng, average_range, average_gauss;
  int convergence_scale;

  rng_t* default_rng() const { return default_rng_; }

  bool      roll( double chance ) const;
  double    range( double min, double max );
  double    gauss( double mean, double stddev );
  timespan_t gauss( timespan_t mean, timespan_t stddev );
  double    real() const;
  rng_t*    get_rng( const std::string& name, int type=RNG_DEFAULT );

  // Timing Wheel Event Management
  event_t** timing_wheel;
  int    wheel_seconds, wheel_size, wheel_mask, timing_slice;
  double wheel_granularity;

  // Raid Events
  std::vector<raid_event_t*> raid_events;
  std::string raid_events_str;
  std::string fight_style;

  // Buffs and Debuffs Overrides
  struct overrides_t
  {
    // Buff overrides
    int attack_haste;
    int attack_power_multiplier;
    int critical_strike;
    int mastery;
    int spell_haste;
    int spell_power_multiplier;
    int stamina;
    int str_agi_int;

    // Debuff overrides
    int slowed_casting;
    int magic_vulnerability;
    int mortal_wounds;
    int physical_vulnerability;
    int ranged_vulnerability;
    int weakened_armor;
    int weakened_blows;

    // Misc stuff needs resolving
    int bloodlust;
    int honor_among_thieves;
  };
  overrides_t overrides;

  // Auras
  struct auras_t
  {
    // Raid-wide auras from various classes
    aura_t* attack_haste;
    aura_t* attack_power_multiplier;
    aura_t* critical_strike;
    aura_t* mastery;
    aura_t* spell_haste;
    aura_t* spell_power_multiplier;
    aura_t* stamina;
    aura_t* str_agi_int;

    // Honor Among Thieves hackery
    aura_t* honor_among_thieves;
  };
  auras_t auras;

  // Auras and De-Buffs
  std::vector<buff_t*> buff_list;
  timespan_t aura_delay;

  // Global aura related delay
  timespan_t default_aura_delay;
  timespan_t default_aura_delay_stddev;

  cooldown_t* cooldown_list;

  // Reporting
  report_t*  report;
  scaling_t* scaling;
  plot_t*    plot;
  reforge_plot_t* reforge_plot;
  timespan_t elapsed_cpu;
  double     iteration_dmg, iteration_heal;
  sample_data_t raid_dps, total_dmg, raid_hps, total_heal, simulation_length;
  int        report_progress;
  int        bloodlust_percent;
  timespan_t bloodlust_time;
  std::string reference_player_str;
  std::vector<player_t*> players_by_dps;
  std::vector<player_t*> players_by_hps;
  std::vector<player_t*> players_by_name;
  std::vector<player_t*> targets_by_name;
  std::vector<std::string> id_dictionary;
  std::vector<timespan_t> iteration_timeline;
  std::vector<int> divisor_timeline;
  std::string output_file_str, html_file_str;
  std::string xml_file_str, xml_stylesheet_file_str;
  std::string path_str;
  std::stack<std::string> active_files;
  std::vector<std::string> error_list;
  FILE* output_file;
  int debug_exp;
  int report_precision;
  int report_pets_separately;
  int report_targets;
  int report_details;
  int report_rng;
  int hosted_html;
  int print_styles;
  int report_overheal;
  int save_raid_summary;
  int statistics_level;
  int separate_stats_by_actions;

  struct report_information_t
  {
    bool charts_generated;
    std::vector<std::string> dps_charts, hps_charts, gear_charts, dpet_charts;
    std::string timeline_chart;
  } report_information;

  std::unordered_map<std::string, std::pair<player_type_e, size_t> > targetdata_items[DATA_COUNT];
  std::vector<std::pair<size_t, std::string> > targetdata_dots[PLAYER_MAX];

  // Multi-Threading
  int threads;
  std::vector<sim_t*> children;
  int thread_index;
  virtual void run() { iterate(); }

  // Spell database access
  spell_data_expr_t* spell_query;
  unsigned           spell_query_level;

  sim_t( sim_t* parent=0, int thrdID=0 );
  virtual ~sim_t();

  int       main( int argc, char** argv );
  void      cancel();
  double    progress( std::string& phase );
  void      combat( int iteration );
  void      combat_begin();
  void      combat_end();
  void      add_event( event_t*, timespan_t delta_time );
  void      reschedule_event( event_t* );
  void      flush_events();
  void      cancel_events( player_t* );
  event_t*  next_event();
  void      reset();
  bool      init();
  void      analyze_player( player_t* p );
  void      analyze();
  void      merge( sim_t& other_sim );
  void      merge();
  bool      iterate();
  void      partition();
  bool      execute();
  void      print_options();
  void      create_options();
  bool      parse_option( const std::string& name, const std::string& value );
  bool      parse_options( int argc, char** argv );
  bool      time_to_think( timespan_t proc_time );
  timespan_t total_reaction_time ();
  double    iteration_adjust();
  player_t* find_player( const std::string& name ) const;
  player_t* find_player( int index ) const;
  cooldown_t* get_cooldown( const std::string& name );
  void      use_optimal_buffs_and_debuffs( int value );
  expr_t* create_expression( action_t*, const std::string& name );
  int       errorf( const char* format, ... ) PRINTF_ATTRIBUTE( 2,3 );
  void register_targetdata_item( int kind, const char* name, player_type_e type, size_t offset );
  void* get_targetdata_item( player_t* source, player_t* target, int kind, const std::string& name );

  dot_t* get_targetdata_dot( player_t* source, player_t* target, const std::string& name )
  {
    return ( dot_t* )get_targetdata_item( source, target, DATA_DOT, name );
  }

  buff_t* get_targetdata_aura( player_t* source, player_t* target, const std::string& name )
  {
    return ( buff_t* )get_targetdata_item( source, target, DATA_AURA, name );
  }
};

// Scaling ==================================================================

struct scaling_t
{
  sim_t* sim;
  sim_t* baseline_sim;
  sim_t* ref_sim;
  sim_t* delta_sim;
  sim_t* ref_sim2;
  sim_t* delta_sim2;
  stat_type_e scale_stat;
  double scale_value;
  double scale_delta_multiplier;
  int    calculate_scale_factors;
  int    center_scale_delta;
  int    positive_scale_delta;
  int    scale_lag;
  double scale_factor_noise;
  int    normalize_scale_factors;
  int    smooth_scale_factors;
  int    debug_scale_factors;
  std::string scale_only_str;
  stat_type_e current_scaling_stat;
  int num_scaling_stats, remaining_scaling_stats;
  double    scale_haste_iterations, scale_expertise_iterations, scale_crit_iterations, scale_hit_iterations, scale_mastery_iterations;
  std::string scale_over;
  std::string scale_over_player;

  // Gear delta for determining scale factors
  gear_stats_t stats;

  scaling_t( sim_t* s );

  void init_deltas();
  void analyze();
  void analyze_stats();
  void analyze_ability_stats( stat_type_e, double, player_t*, player_t*, player_t* );
  void analyze_lag();
  void normalize();
  void derive();
  double progress( std::string& phase );
  void create_options();
  bool has_scale_factors();
  double scale_over_function( sim_t* s, player_t* p );
  double scale_over_function_error( sim_t* s, player_t* p );
};

// Plot =====================================================================

struct plot_t
{
  sim_t* sim;
  std::string dps_plot_stat_str;
  double dps_plot_step;
  int    dps_plot_points;
  int    dps_plot_iterations;
  int    dps_plot_debug;
  stat_type_e current_plot_stat;
  int    num_plot_stats, remaining_plot_stats, remaining_plot_points;
  bool	 dps_plot_positive;

  plot_t( sim_t* s );

  void analyze();
  void analyze_stats();
  double progress( std::string& phase );
  void create_options();
};

// Reforge Plot =============================================================

struct reforge_plot_t
{
  sim_t* sim;
  sim_t* current_reforge_sim;
  std::string reforge_plot_stat_str;
  std::string reforge_plot_output_file_str;
  FILE* reforge_plot_output_file;
  std::vector<stat_type_e> reforge_plot_stat_indices;
  int    reforge_plot_step;
  int    reforge_plot_amount;
  int    reforge_plot_iterations;
  int    reforge_plot_debug;
  int    current_stat_combo;
  int    num_stat_combos;

  reforge_plot_t( sim_t* s );

  void generate_stat_mods( std::vector<std::vector<int> > &stat_mods,
                           const std::vector<stat_type_e> &stat_indices,
                           int cur_mod_stat,
                           std::vector<int> cur_stat_mods );
  void analyze();
  void analyze_stats();
  double progress( std::string& phase );
  void create_options();
};

struct reforge_plot_data_t
{
  double value;
  double error;
};

// Event ====================================================================

struct event_t : public noncopyable
{
private:
  static void cancel_( event_t* e );
  static void early_( event_t* e );

  static void* operator new( std::size_t ) throw(); // DO NOT USE!

public:
  event_t*  next;
  sim_t* const sim;
  player_t* const player;
  uint32_t  id;
  timespan_t time;
  timespan_t reschedule_time;
  int       canceled;
  const char* name;
  event_t( sim_t* s, player_t* p=0, const char* n="" ) :
    next( 0 ), sim( s ), player( p ), time( timespan_t::zero() ), reschedule_time( timespan_t::zero() ), canceled( 0 ), name( n )
  {
    if ( ! name ) name = "unknown";
  }
  timespan_t occurs()  const { return ( reschedule_time != timespan_t::zero() ) ? reschedule_time : time; }
  timespan_t remains() const { return occurs() - sim -> current_time; }
  virtual void reschedule( timespan_t new_time );
  virtual void execute() = 0;
  virtual ~event_t() {}

  // T must be implicitly convertible to event_t* --
  // basically, a pointer to a type derived from event_t.
  template <typename T> static void cancel( T& e )
  { if ( e ) { cancel_( e ); e = 0; } }
  template <typename T> static void early( T& e )
  { if ( e ) { early_( e ); e = 0; } }

  // Simple free-list memory manager.
  static void* operator new( std::size_t size, sim_t* sim )
  { return sim -> free_list.allocate( size ); }

  static void operator delete( void* p )
  {
    event_t* e = static_cast<event_t*>( p );
    e -> sim -> free_list.deallocate( e );
  }

  static void operator delete( void* p, sim_t* sim )
  { sim -> free_list.deallocate( p ); }
};

// Gear Rating Conversions ==================================================

namespace internal {
struct rating_t
{
  double  spell_haste,  spell_hit,  spell_crit;
  double attack_haste, attack_hit, attack_crit;
  double ranged_haste, ranged_hit,ranged_crit;
  double expertise;
  double dodge, parry, block;
  double mastery;
};
}

struct rating_t : public internal::rating_t
{
  typedef internal::rating_t base_t;
  rating_t() : base_t( base_t() ) {}

  void init( sim_t*, dbc_t& pData, int level, player_type_e type );
  static double interpolate( int level, double val_60, double val_70, double val_80, double val_85 = -1 );
  static double get_attribute_base( sim_t*, dbc_t& pData, int level, player_type_e class_type, race_type_e race, base_stat_type_e_e stat_type_e );
};

// Weapon ===================================================================

struct weapon_t
{
  weapon_type_e type;
  school_type_e school;
  double damage, dps;
  double min_dmg, max_dmg;
  timespan_t swing_time;
  slot_type_e slot;
  int    buff_type;
  double buff_value;
  double bonus_dmg;

  weapon_type_e group() const;
  timespan_t normalized_weapon_speed() const;
  double proc_chance_on_swing( double PPM, timespan_t adjusted_swing_time=timespan_t::zero() ) const;

  weapon_t( weapon_type_e t=WEAPON_NONE,
            double d=0,
            timespan_t st=timespan_t::from_seconds( 2.0 ),
            school_type_e s=SCHOOL_PHYSICAL ) :
    type( t ), school( s ), damage( d ), min_dmg( d ), max_dmg( d ),
    swing_time( st ), slot( SLOT_INVALID ), buff_type( 0 ), buff_value( 0 ),
    bonus_dmg( 0 ) {}
};

// Item =====================================================================

struct item_t
{
  sim_t* sim;
  player_t* player;
  slot_type_e slot;
  int quality, ilevel;
  bool unique, unique_enchant, unique_addon, is_heroic, is_lfr, is_ptr, is_matching_type, is_reforged;
  stat_type_e reforged_from;
  stat_type_e reforged_to;

  // Option Data
  std::string option_name_str;
  std::string option_id_str;
  std::string option_stats_str;
  std::string option_gems_str;
  std::string option_enchant_str;
  std::string option_addon_str;
  std::string option_equip_str;
  std::string option_use_str;
  std::string option_weapon_str;
  std::string option_heroic_str;
  std::string option_lfr_str;
  std::string option_armor_type_str;
  std::string option_reforge_str;
  std::string option_random_suffix_str;
  std::string option_ilevel_str;
  std::string option_quality_str;
  std::string option_data_source_str;
  std::string options_str;

  // Armory Data
  std::string armory_name_str;
  std::string armory_id_str;
  std::string armory_stats_str;
  std::string armory_gems_str;
  std::string armory_enchant_str;
  std::string armory_addon_str;
  std::string armory_weapon_str;
  std::string armory_heroic_str;
  std::string armory_lfr_str;
  std::string armory_armor_type_str;
  std::string armory_reforge_str;
  std::string armory_ilevel_str;
  std::string armory_quality_str;
  std::string armory_random_suffix_str;

  // Encoded Data
  std::string id_str;
  std::string encoded_name_str;
  std::string encoded_stats_str;
  std::string encoded_gems_str;
  std::string encoded_enchant_str;
  std::string encoded_addon_str;
  std::string encoded_equip_str;
  std::string encoded_use_str;
  std::string encoded_weapon_str;
  std::string encoded_heroic_str;
  std::string encoded_lfr_str;
  std::string encoded_armor_type_str;
  std::string encoded_reforge_str;
  std::string encoded_ilevel_str;
  std::string encoded_quality_str;
  std::string encoded_random_suffix_str;

  // Extracted data
  gear_stats_t base_stats,stats;
  struct special_effect_t
  {
    std::string name_str, trigger_str;
    proc_type_e trigger_type;
    int64_t trigger_mask;
    stat_type_e stat;
    school_type_e school;
    int max_stacks;
    double stat_amount, discharge_amount, discharge_scaling;
    double proc_chance;
    timespan_t duration, cooldown, tick;
    bool cost_reduction;
    bool no_player_benefits;
    bool no_debuffs;
    bool no_refresh;
    bool chance_to_discharge;
    unsigned int override_result_type_es_mask;
    unsigned result_type_es_mask;
    bool reverse;
    special_effect_t() :
      trigger_type( PROC_NONE ), trigger_mask( 0 ), stat( STAT_NONE ), school( SCHOOL_NONE ),
      max_stacks( 0 ), stat_amount( 0 ), discharge_amount( 0 ), discharge_scaling( 0 ),
      proc_chance( 0 ), duration( timespan_t::zero() ), cooldown( timespan_t::zero() ),
      tick( timespan_t::zero() ), cost_reduction( false ), no_player_benefits( false ), no_debuffs( false ),
      no_refresh( false ), chance_to_discharge( false ), override_result_type_es_mask( 0 ), result_type_es_mask( 0 ), reverse( false ) {}
    bool active() { return stat || school; }
  } use, equip, enchant, addon;

  item_t() : sim( 0 ), player( 0 ), slot( SLOT_INVALID ), quality( 0 ), ilevel( 0 ), unique( false ), unique_enchant( false ),
    unique_addon( false ), is_heroic( false ), is_lfr( false ), is_ptr( false ), is_matching_type( false ), is_reforged( false ) {}
  item_t( player_t*, const std::string& options_str );
  bool active() const;
  bool heroic() const;
  bool lfr() const;
  bool ptr() const;
  bool reforged() const;
  bool matching_type();
  const char* name() const;
  const char* slot_name() const;
  const char* armor_type();
  weapon_t* weapon() const;
  bool init();
  bool parse_options();
  void encode_options();
  bool decode_stats();
  bool decode_gems();
  bool decode_enchant();
  bool decode_addon();
  bool decode_special( special_effect_t&, const std::string& encoding );
  bool decode_weapon();
  bool decode_heroic();
  bool decode_lfr();
  bool decode_armor_type();
  bool decode_reforge();
  bool decode_random_suffix();
  bool decode_ilevel();
  bool decode_quality();

  static bool download_slot( item_t& item,
                             const std::string& item_id,
                             const std::string& enchant_id,
                             const std::string& addon_id,
                             const std::string& reforge_id,
                             const std::string& rsuffix_id,
                             const std::string gem_ids[ 3 ] );
  static bool download_item( item_t&, const std::string& item_id );
  static bool download_glyph( player_t* player, std::string& glyph_name, const std::string& glyph_id );
  static gem_type_e parse_gem( item_t&            item,
                               const std::string& gem_id );
};

// Item database ============================================================
struct item_database_t
{
  static bool     download_slot(      item_t& item,
                                      const std::string& item_id,
                                      const std::string& enchant_id,
                                      const std::string& addon_id,
                                      const std::string& reforge_id,
                                      const std::string& rsuffix_id,
                                      const std::string gem_ids[ 3 ] );
  static bool     download_item(      item_t& item, const std::string& item_id );
  static bool     download_glyph(     player_t* player, std::string& glyph_name, const std::string& glyph_id );
  static gem_type_e parse_gem(        item_t& item, const std::string& gem_id );
  static bool     initialize_item_sources( const item_t& item, std::vector<std::string>& source_list );

  static int      random_suffix_type( const item_t& item );
  static int      random_suffix_type( const item_data_t* );
  static uint32_t armor_value(        const item_t& item, unsigned item_id );
  static uint32_t armor_value(        const item_data_t*, const dbc_t& );
  static uint32_t weapon_dmg_min(     const item_t& item, unsigned item_id );
  static uint32_t weapon_dmg_min(     const item_data_t*, const dbc_t& );
  static uint32_t weapon_dmg_max(     const item_t& item, unsigned item_id );
  static uint32_t weapon_dmg_max(     const item_data_t*, const dbc_t& );

  static bool     load_item_from_data( item_t& item, const item_data_t* item_data );
  static bool     parse_gems(         item_t&            item,
                                      const item_data_t* item_data,
                                      const std::string  gem_ids[ 3 ] );
  static bool     parse_enchant(      item_t& item, const std::string& enchant_id );
};

// Set Bonus ================================================================

struct set_bonus_t
{
  std::array<int,SET_MAX> count;
  int tier13_2pc_caster() const; int tier13_2pc_melee() const; int tier13_2pc_tank() const; int tier13_2pc_heal() const;
  int tier13_4pc_caster() const; int tier13_4pc_melee() const; int tier13_4pc_tank() const; int tier13_4pc_heal() const;
  int tier14_2pc_caster() const; int tier14_2pc_melee() const; int tier14_2pc_tank() const; int tier14_2pc_heal() const;
  int tier14_4pc_caster() const; int tier14_4pc_melee() const; int tier14_4pc_tank() const; int tier14_4pc_heal() const;
  int tier15_2pc_caster() const; int tier15_2pc_melee() const; int tier15_2pc_tank() const; int tier15_2pc_heal() const;
  int tier15_4pc_caster() const; int tier15_4pc_melee() const; int tier15_4pc_tank() const; int tier15_4pc_heal() const;
  int tier16_2pc_caster() const; int tier16_2pc_melee() const; int tier16_2pc_tank() const; int tier16_2pc_heal() const;
  int tier16_4pc_caster() const; int tier16_4pc_melee() const; int tier16_4pc_tank() const; int tier16_4pc_heal() const;
  int pvp_2pc_caster() const; int pvp_2pc_melee() const; int pvp_2pc_tank() const; int pvp_2pc_heal() const;
  int pvp_4pc_caster() const; int pvp_4pc_melee() const; int pvp_4pc_tank() const; int pvp_4pc_heal() const;
  int decode( player_t*, item_t& item ) const;
  bool init( player_t* );

  set_bonus_t();

  expr_t* create_expression( player_t*, const std::string& type );
};

struct set_bonus_array_t
{
private:
  const spell_data_t* default_value;
  const spell_data_t* set_bonuses[ SET_MAX ];
  player_t* p;

  const spell_data_t* create_set_bonus( uint32_t spell_id );

public:
  set_bonus_array_t( player_t* p, const uint32_t a_bonus[ N_TIER ][ N_TIER_BONUS ] );

  bool              has_set_bonus( set_type_e s ) const;
  const spell_data_t* set( set_type_e s ) const;
};

// Player ===================================================================

struct player_t : public noncopyable
{
  sim_t* const sim;
  const player_type_e type;
  const std::string name_str;
  race_type_e race;
  std::string talents_str, glyphs_str, id_str, target_str;
  std::string region_str, server_str, origin_str;
  player_t*   next;
  int         index;
  role_type_e   role;
  player_t*   target;
  int         level, use_pre_potion, party, member;
  double      skill, initial_skill, distance, default_distance;
  timespan_t  gcd_ready;
  timespan_t  base_gcd;
  int         potion_used, sleeping, initial_sleeping, initialized;
  rating_t    rating;
  std::vector<pet_t*> pet_list;
  int         bugs;
  specialization_e spec;
  int         invert_scaling;
  bool        vengeance_enabled;
  double      vengeance_damage, vengeance_value, vengeance_max;
  bool        vengeance_was_attacked;
  int         active_pets;
  double      dtr_proc_chance;
  double      dtr_base_proc_chance;
  timespan_t  reaction_mean,reaction_stddev,reaction_nu;
  std::array<int, RESOURCE_MAX> infinite_resource;
  std::vector<buff_t*> absorb_buffs;
  int         scale_player;
  bool        has_dtr;
  double      avg_ilvl;

  // Latency
  timespan_t  world_lag, world_lag_stddev;
  timespan_t  brain_lag, brain_lag_stddev;
  bool        world_lag_override, world_lag_stddev_override;

  int    events;

  // Data access
  dbc_t       dbc;

  // Option Parsing
  std::vector<option_t> options;

  // Misc values
  int autoUnshift;

  // Talent Parsing
  std::array<uint32_t,MAX_TALENT_SLOTS> talent_list;

  // Glyph Parsing
  std::vector<const spell_data_t*> glyph_list;

  // Profs
  std::string professions_str;
  std::array<int,PROFESSION_MAX> profession;

  // Race
  std::string race_str;

  // Haste
  double base_haste_rating, initial_haste_rating, haste_rating;
  double spell_haste, attack_haste;

  // Attributes
  std::array<double,ATTRIBUTE_MAX> attribute, attribute_base, attribute_initial,
                                   attribute_multiplier, attribute_multiplier_initial;

  double mastery, mastery_rating, initial_mastery_rating,base_mastery;

  // Spell Mechanics
  double base_spell_power;
  std::array<double,SCHOOL_MAX + 1> initial_spell_power;
  std::array<double,SCHOOL_MAX + 1> spell_power;
  double base_spell_hit,         initial_spell_hit,                   spell_hit;
  double base_spell_crit,        initial_spell_crit,                  spell_crit;
  double base_mp5,               initial_mp5,                         mp5;
  double spell_power_multiplier,    initial_spell_power_multiplier;
  double spell_power_per_intellect, initial_spell_power_per_intellect;
  double spell_crit_per_intellect,  initial_spell_crit_per_intellect;
  double mana_regen_base;
  double base_energy_regen_per_second;
  double base_focus_regen_per_second;
  double base_chi_regen_per_second;
  std::array<double,SCHOOL_MAX> resource_reduction, initial_resource_reduction;
  timespan_t last_cast;

  // Attack Mechanics
  double base_attack_power,       initial_attack_power,        attack_power;
  double base_attack_hit,         initial_attack_hit,          attack_hit;
  double base_attack_expertise,   initial_attack_expertise,    attack_expertise;
  double base_attack_crit,        initial_attack_crit,         attack_crit;
  double attack_power_multiplier,   initial_attack_power_multiplier;
  double attack_power_per_strength, initial_attack_power_per_strength;
  double attack_power_per_agility,  initial_attack_power_per_agility;
  double attack_crit_per_agility,   initial_attack_crit_per_agility;
  position_type_e position;
  std::string position_str;

  // Defense Mechanics
  event_t* target_auto_attack;
  double base_armor,       initial_armor,       armor;
  double base_bonus_armor, initial_bonus_armor, bonus_armor;
  double base_miss,        initial_miss,        miss;
  double base_dodge,       initial_dodge,       dodge;
  double base_parry,       initial_parry,       parry;
  double base_block,       initial_block,       block;
  double base_block_reduction, initial_block_reduction, block_reduction;
  double armor_multiplier,  initial_armor_multiplier;
  double dodge_per_agility, initial_dodge_per_agility;
  double parry_rating_per_strength, initial_parry_rating_per_strength;
  double diminished_dodge_capi, diminished_parry_capi, diminished_kfactor;
  double armor_coeff;
  double half_resistance_rating;
  std::array< int, SCHOOL_MAX > spell_resistance;

  // Weapons
  weapon_t main_hand_weapon;
  weapon_t off_hand_weapon;
  weapon_t ranged_weapon;

  // Main, offhand, and ranged attacks
  attack_t* main_hand_attack;
  attack_t*  off_hand_attack;
  attack_t* ranged_attack;

  // Resources
  struct resources_t
  {
    std::array<double,RESOURCE_MAX> base, initial, max, current,
                                    base_multiplier, initial_multiplier;

    resources_t()
    {
      range::fill( base, 0.0 ); range::fill( initial, 0.0 ); range::fill( max, 0.0 ); range::fill( current, 0.0 );
      range::fill( base_multiplier, 1.0 ); range::fill( initial_multiplier, 1.0 );
    }
  } resources;

  double  mana_per_intellect;
  double mp5_from_spirit_multiplier;
  uptime_t* primary_resource_cap;

  // Consumables
  std::string flask_str, elixirs_str, food_str;
  int elixir_guardian;
  int elixir_battle;
  int flask;
  int food;

  // Events
  action_t* executing;
  action_t* channeling;
  event_t*  readying;
  event_t*  off_gcd;
  bool      in_combat;
  bool      action_queued;

  // Delay time used by "cast_delay" expression to determine when an action
  // can be used at minimum after a spell cast has finished, including GCD
  timespan_t cast_delay_reaction;
  timespan_t cast_delay_occurred;

  // Callbacks
  std::vector<action_callback_t*> all_callbacks;
  std::array< std::vector<action_callback_t*>, RESULT_MAX > attack_callbacks;
  std::array< std::vector<action_callback_t*>, RESULT_MAX > spell_callbacks;
  std::array< std::vector<action_callback_t*>, RESULT_MAX > harmful_spell_callbacks;
  std::array< std::vector<action_callback_t*>, RESULT_MAX > heal_callbacks;
  std::array< std::vector<action_callback_t*>, RESULT_MAX > absorb_callbacks;
  std::array< std::vector<action_callback_t*>, RESULT_MAX > tick_callbacks;

  std::array< std::vector<action_callback_t*>, SCHOOL_MAX > direct_damage_callbacks;
  std::array< std::vector<action_callback_t*>, SCHOOL_MAX > tick_damage_callbacks;
  std::array< std::vector<action_callback_t*>, SCHOOL_MAX > direct_heal_callbacks;
  std::array< std::vector<action_callback_t*>, SCHOOL_MAX > tick_heal_callbacks;

  std::array< std::vector<action_callback_t*>, RESOURCE_MAX > resource_gain_callbacks;
  std::array< std::vector<action_callback_t*>, RESOURCE_MAX > resource_loss_callbacks;

  // Action Priority List
  std::vector<action_t*>  action_list;
  std::vector<action_t*>  foreground_action_list;
  std::vector<action_t*> off_gcd_actions;
  std::string action_list_str;
  std::string choose_action_list;
  std::string action_list_skip;
  std::string modify_action;
  int         action_list_default;
  cooldown_t* cooldown_list;
  std::vector<dot_t*> dot_list;
  std::map<std::string,int> action_map;
  std::vector<action_priority_list_t*> action_priority_list;

  // Reporting
  int       quiet;
  action_t* last_foreground_action;
  timespan_t iteration_fight_length,arise_time;
  sample_data_t fight_length, waiting_time, executed_foreground_actions;
  timespan_t iteration_waiting_time;
  int       iteration_executed_foreground_actions;
  std::array< double, RESOURCE_MAX > resource_lost, resource_gained;
  double    rps_gain, rps_loss;
  sample_data_t deaths;
  double    deaths_error;

  // Buffed snapshot_stats (for reporting)
  struct buffed_stats_t
  {
    std::array< double, ATTRIBUTE_MAX > attribute;
    std::array< double, RESOURCE_MAX > resource;

    double spell_power, spell_hit, spell_crit, mp5;

    double attack_power,  attack_hit,  mh_attack_expertise,  oh_attack_expertise, attack_crit;

    double armor, miss, crit, dodge, parry, block;

    double spell_haste, attack_haste, attack_speed;

    double mastery;

  } buffed;

  std::vector<buff_t*> buff_list;
  proc_t*   proc_list;
  gain_t*   gain_list;
  std::vector<stats_t*> stats_list;
  benefit_t* benefit_list;
  uptime_t* uptime_list;
  std::array< std::vector<double>, STAT_MAX > dps_plot_data;
  std::vector<std::vector<reforge_plot_data_t> > reforge_plot_data;

  struct resource_timeline_t
  {
    resource_type_e type;
    std::vector<double> timeline;

    resource_timeline_t( resource_type_e t = RESOURCE_NONE ) : type( t ) {}
  };

   // Health/primary/secondary makes 3 resources, does any class/spec need more?
  std::array<resource_timeline_t,3> resource_timelines;
  size_t resource_timeline_count;

  // Damage
  double iteration_dmg, iteration_dmg_taken;
  double dps_error, dpr, dtps_error;
  sample_data_t dmg;
  sample_data_t compound_dmg;
  sample_data_t dps;
  sample_data_t dpse;
  sample_data_t dtps;
  sample_data_t dmg_taken;
  std::vector<double> timeline_dmg;
  std::vector<double> timeline_dps;
  std::vector<double> dps_convergence_error;
  double    dps_convergence;

  // Heal
  double iteration_heal,iteration_heal_taken;
  double hps_error,hpr;
  sample_data_t heal;
  sample_data_t compound_heal;
  sample_data_t hps;
  sample_data_t hpse;
  sample_data_t htps;
  sample_data_t heal_taken;

  struct report_information_t
  {
    bool charts_generated, buff_lists_generated;
    std::string action_sequence;
    std::string action_dpet_chart, action_dmg_chart, time_spent_chart;
    std::array<std::string, RESOURCE_MAX> timeline_resource_chart, gains_chart;
    std::string timeline_dps_chart, timeline_dps_error_chart, timeline_resource_health_chart;
    std::string distribution_dps_chart, scaling_dps_chart, scale_factors_chart;
    std::string reforge_dps_chart, dps_error_chart, distribution_deaths_chart;
    std::string gear_weights_lootrank_link, gear_weights_wowhead_link, gear_weights_wowreforge_link;
    std::string gear_weights_pawn_std_string, gear_weights_pawn_alt_string;
    std::string save_str;
    std::string save_gear_str;
    std::string save_talents_str;
    std::string save_actions_str;
    std::string comment_str;
    std::string thumbnail_url;
    std::vector<buff_t*> buff_list, dynamic_buffs, constant_buffs;

    report_information_t() { charts_generated = buff_lists_generated = false; }
  } report_information;

  // Gear
  std::string items_str, meta_gem_str;
  std::vector<item_t> items;
  gear_stats_t stats, initial_stats, gear, enchant, temporary;
  set_bonus_t set_bonus;
  set_bonus_array_t * sets;
  meta_gem_type_e meta_gem;
  bool matching_gear;

  // Scale Factors
  gear_stats_t scaling;
  gear_stats_t scaling_normalized;
  gear_stats_t scaling_error;
  gear_stats_t scaling_delta_dps;
  gear_stats_t scaling_compare_error;
  double       scaling_lag, scaling_lag_error;
  std::array<bool,STAT_MAX> scales_with;
  std::array<double,STAT_MAX> over_cap;
  std::vector<stat_type_e> scaling_stats; // sorting vector

  // Movement & Position
  double base_movement_speed;
  double x_position, y_position;

  struct buffs_t
  {
    buff_t* beacon_of_light;
    buff_t* berserking;
    buff_t* blood_fury_ap;
    buff_t* blood_fury_sp;
    buff_t* bloodlust;
    buff_t* body_and_soul;
    buff_t* exhaustion;
    buff_t* grace;
    buff_t* guardian_spirit;
    buff_t* heroic_presence;
    buff_t* hymn_of_hope;
    buff_t* illuminated_healing;
    buff_t* innervate;
    buff_t* lifeblood;
    buff_t* mana_tide;
    buff_t* mongoose_mh;
    buff_t* mongoose_oh;
    buff_t* pain_supression;
    buff_t* power_infusion;
    buff_t* raid_movement;
    buff_t* self_movement;
    buff_t* stoneform;
    buff_t* stunned;
    buff_t* tricks_of_the_trade;
    buff_t* unholy_frenzy;
    buff_t* weakened_soul;
  } buffs;

  struct potion_buffs_t
  {
    stat_buff_t* earthen;
    stat_buff_t* golemblood;
    stat_buff_t* speed;
    stat_buff_t* tolvir;
    stat_buff_t* volcanic;

    // new mo
    stat_buff_t* virmens_bite; // agi
    stat_buff_t* mogu_power; // str
    stat_buff_t* jinyu; // int
    stat_buff_t* mountains; // armor
  } potion_buffs;

  struct debuffs_t
  {
    debuff_t* bleeding;
    debuff_t* casting;
    debuff_t* flying;
    debuff_t* forbearance;
    debuff_t* invulnerable;
    debuff_t* vulnerable;

    // MoP debuffs
    debuff_t* slowed_casting;
    debuff_t* magic_vulnerability;
    debuff_t* mortal_wounds;
    debuff_t* physical_damage;
    debuff_t* physical_vulnerability;
    debuff_t* ranged_vulnerability;
    debuff_t* weakened_blows;
    debuff_t* weakened_armor;

    // Class specific "general" debuffs
    debuff_t* shattering_throw;
  } debuffs;

  struct gains_t
  {
    gain_t* arcane_torrent;
    gain_t* blessing_of_might;
    gain_t* chi_regen;
    gain_t* dark_rune;
    gain_t* energy_regen;
    gain_t* essence_of_the_red;
    gain_t* focus_regen;
    gain_t* glyph_of_innervate;
    gain_t* hymn_of_hope;
    gain_t* innervate;
    gain_t* mana_potion;
    gain_t* mana_spring_totem;
    gain_t* mp5_regen;
    gain_t* restore_mana;
    gain_t* spellsurge;
    gain_t* vampiric_embrace;
    gain_t* vampiric_touch;
    gain_t* water_elemental;
  } gains;

  struct procs_t
  {
    proc_t* hat_donor;
  } procs;

  rng_t* rng_list;

  struct rngs_t
  {
    rng_t* lag_ability;
    rng_t* lag_brain;
    rng_t* lag_channel;
    rng_t* lag_gcd;
    rng_t* lag_queue;
    rng_t* lag_reaction;
    rng_t* lag_world;
  } rngs;

  int targetdata_id;
  std::vector<targetdata_t*> targetdata;

  player_t( sim_t* sim, player_type_e type, const std::string& name, race_type_e race_type_e = RACE_NONE );

  virtual ~player_t();

  virtual const char* name() const { return name_str.c_str(); }

  virtual targetdata_t* new_targetdata( player_t* target );
  virtual void init();
  virtual void init_glyphs();
  virtual void init_base() = 0;
  virtual void init_items();
  virtual void init_meta_gem( gear_stats_t& );
  virtual void init_core();
  virtual void init_position();
  virtual void init_race();
  virtual void init_racials();
  virtual void init_spell();
  virtual void init_attack();
  virtual void init_defense();
  virtual void init_weapon( weapon_t* );
  virtual void init_unique_gear();
  virtual void init_enchant();
  virtual void init_resources( bool force = false );
  virtual void init_professions();
  virtual void init_professions_bonus();
  virtual std::string init_use_item_actions( const std::string& append = std::string() );
  virtual std::string init_use_profession_actions( const std::string& append = std::string() );
  virtual std::string init_use_racial_actions( const std::string& append = std::string() );
  virtual void init_actions();
  virtual void init_rating();
  virtual void init_scaling();
  virtual void init_spells();
  virtual void init_buffs();
  virtual void init_gains();
  virtual void init_procs();
  virtual void init_uptimes();
  virtual void init_benefits();
  virtual void init_rng();
  virtual void init_stats();
  virtual void init_values();
  virtual void init_target();

  virtual void reset();
  virtual void combat_begin();
  virtual void combat_end();
  virtual void merge( player_t& other );

  virtual double energy_regen_per_second() const;
  virtual double focus_regen_per_second() const;
  virtual double chi_regen_per_second() const;
  virtual double composite_attack_haste() const;
  virtual double composite_attack_speed() const;
  virtual double composite_attack_power() const;
  virtual double composite_attack_crit( const weapon_t* = 0 ) const;
  virtual double composite_attack_expertise( const weapon_t* = 0 ) const;
  virtual double composite_attack_hit() const;

  virtual double composite_spell_haste() const;
  virtual double composite_spell_power( school_type_e school ) const;
  virtual double composite_spell_crit() const;
  virtual double composite_spell_hit() const;
  virtual double composite_mp5() const;
  virtual double composite_mastery() const;

  virtual double composite_armor()                 const;
  virtual double composite_armor_multiplier()      const;
  virtual double composite_spell_resistance( school_type_e school ) const;
  virtual double composite_tank_miss( school_type_e school ) const;
  virtual double composite_tank_dodge()            const;
  virtual double composite_tank_parry()            const;
  virtual double composite_tank_block()            const;
  virtual double composite_tank_block_reduction()  const;
  virtual double composite_tank_crit_block()            const;
  virtual double composite_tank_crit( school_type_e school ) const;

  virtual double diminished_dodge()             const;
  virtual double diminished_parry()             const;

  virtual double composite_attack_power_multiplier() const;
  virtual double composite_spell_power_multiplier() const;

  virtual double matching_gear_multiplier( attribute_type_e /* attr */ ) const { return 0; }

  virtual double composite_player_multiplier   ( school_type_e, const action_t* a = NULL ) const;
  virtual double composite_player_dd_multiplier( school_type_e, const action_t* /* a */ = NULL ) const { return 1; }
  virtual double composite_player_td_multiplier( school_type_e, const action_t* a = NULL ) const;

  virtual double composite_player_heal_multiplier( school_type_e school ) const;
  virtual double composite_player_dh_multiplier( school_type_e /* school */ ) const { return 1; }
  virtual double composite_player_th_multiplier( school_type_e school ) const;

  virtual double composite_player_absorb_multiplier( school_type_e school ) const;

  virtual double composite_movement_speed() const;

  virtual double composite_attribute( attribute_type_e attr ) const;
  virtual double composite_attribute_multiplier( attribute_type_e attr ) const;

  double get_attribute( attribute_type_e a ) const;
  double strength() const { return get_attribute( ATTR_STRENGTH ); }
  double agility() const { return get_attribute( ATTR_AGILITY ); }
  double stamina() const { return get_attribute( ATTR_STAMINA ); }
  double intellect() const { return get_attribute( ATTR_INTELLECT ); }
  double spirit() const { return get_attribute( ATTR_SPIRIT ); }

  virtual void      interrupt();
  virtual void      halt();
  virtual void      moving();
  virtual void      stun();
  virtual void      clear_debuffs();
  virtual void      schedule_ready( timespan_t delta_time=timespan_t::zero(), bool waiting=false );
  virtual void      arise();
  virtual void      demise();
  virtual timespan_t available() const { return timespan_t::from_seconds( 0.1 ); }
  virtual action_t* execute_action();

  virtual std::string print_action_map( const int iterations, int precision ) const;

  virtual void   regen( timespan_t periodicity=timespan_t::from_seconds( 0.25 ) );
  virtual double resource_gain( resource_type_e resource_type, double amount, gain_t* g=0, action_t* a=0 );
  virtual double resource_loss( resource_type_e resource_type, double amount, action_t* a=0 );
  virtual void   recalculate_resource_max( resource_type_e resource_type );
  virtual bool   resource_available( resource_type_e resource_type, double cost ) const;
  virtual resource_type_e primary_resource() const { return RESOURCE_NONE; }
  virtual role_type_e   primary_role() const;
  virtual specialization_e primary_tree() const;
  virtual const char* primary_tree_name() const;
  virtual stat_type_e normalize_by() const;

  virtual double health_percentage() const;
  virtual timespan_t time_to_die() const;
  virtual timespan_t total_reaction_time() const;

  virtual void stat_gain( stat_type_e stat, double amount, gain_t* g=0, action_t* a=0, bool temporary=false );
  virtual void stat_loss( stat_type_e stat, double amount, action_t* a=0, bool temporary=false );

  virtual void cost_reduction_gain( school_type_e school, double amount, gain_t* g=0, action_t* a=0 );
  virtual void cost_reduction_loss( school_type_e school, double amount, action_t* a=0 );

  virtual double assess_damage( double amount, school_type_e, dmg_type_e, result_type_e, action_t* a=0 );
  virtual double target_mitigation( double amount, school_type_e, dmg_type_e, result_type_e, action_t* a=0 );

  struct heal_info_t { double actual, amount; };
  virtual heal_info_t assess_heal( double amount, school_type_e, dmg_type_e, result_type_e, action_t* a=0 );

  virtual void  summon_pet( const char* name, timespan_t duration=timespan_t::zero() );
  virtual void dismiss_pet( const char* name );

  virtual bool ooc_buffs() { return true; }

  virtual bool is_moving() { return buffs.raid_movement -> check() || buffs.self_movement -> check(); }

  virtual void register_callbacks();
  virtual void register_resource_gain_callback( resource_type_e,       action_callback_t* );
  virtual void register_resource_loss_callback( resource_type_e,       action_callback_t* );
  virtual void register_attack_callback       ( int64_t result_mask, action_callback_t* );
  virtual void register_spell_callback        ( int64_t result_mask, action_callback_t* );
  virtual void register_tick_callback         ( int64_t result_mask, action_callback_t* );
  virtual void register_heal_callback         ( int64_t result_mask, action_callback_t* );
  virtual void register_absorb_callback         ( int64_t result_mask, action_callback_t* );
  virtual void register_harmful_spell_callback( int64_t result_mask, action_callback_t* );
  virtual void register_tick_damage_callback  ( int64_t result_mask, action_callback_t* );
  virtual void register_direct_damage_callback( int64_t result_mask, action_callback_t* );
  virtual void register_tick_heal_callback    ( int64_t result_mask, action_callback_t* );
  virtual void register_direct_heal_callback  ( int64_t result_mask, action_callback_t* );

  virtual bool parse_talents_armory ( const std::string& talent_string );
  virtual bool parse_talents_wowhead( const std::string& talent_string );

  virtual void replace_spells();

  virtual const spell_data_t* find_glyph( const std::string& name );
  virtual const spell_data_t* find_racial_spell( const std::string& name, const std::string& token = std::string(), race_type_e s = RACE_NONE );
  virtual const spell_data_t* find_class_spell( const std::string& name, const std::string& token = std::string(), specialization_e s = SPEC_NONE );
  virtual const spell_data_t* find_pet_spell( const std::string& name, const std::string& token = std::string() );
  virtual const spell_data_t* find_talent_spell( const std::string& name, const std::string& token = std::string() );
  virtual const spell_data_t* find_glyph_spell( const std::string& name, const std::string& token = std::string() );
  virtual const spell_data_t* find_specialization_spell( const std::string& name, const std::string& token = std::string() );
  virtual const spell_data_t* find_mastery_spell( const std::string& name, const std::string& token = std::string() );
  virtual const spell_data_t* find_mastery_spell( specialization_e s, const std::string& token = std::string(), uint32_t idx = 0 );
  virtual const spell_data_t* find_spell( const std::string& name, const std::string& token = std::string(), specialization_e s = SPEC_NONE);
  virtual const spell_data_t* find_spell( const unsigned int id, const std::string& token = std::string() );

  virtual expr_t* create_expression( action_t*, const std::string& name );
  expr_t* create_resource_expression( const std::string& name );

  virtual void create_options();
  virtual bool create_profile( std::string& profile_str, save_type_e=SAVE_ALL, bool save_html=false );

  virtual void copy_from( player_t* source );

  virtual action_t* create_action( const std::string& name, const std::string& options );
  virtual void      create_pets() { }
  virtual pet_t*    create_pet( const std::string& /* name*/,  const std::string& /* type */ = std::string() ) { return 0; }
  virtual pet_t*    find_pet  ( const std::string& name );

  virtual int decode_set( const item_t& item ) const { ( void )item; assert( item.name() ); return SET_NONE; }

  virtual void recalculate_haste();

  virtual void armory_extensions( const std::string& /* region */, const std::string& /* server */, const std::string& /* character */,
                                  cache::behavior_e /* behavior */=cache::players() )
  {}

  // Class-Specific Methods

  static player_t* create( sim_t* sim, const std::string& type, const std::string& name, race_type_e r = RACE_NONE );

  static player_t* create_death_knight( sim_t* sim, const std::string& name, race_type_e r = RACE_NONE );
  static player_t* create_druid       ( sim_t* sim, const std::string& name, race_type_e r = RACE_NONE );
  static player_t* create_hunter      ( sim_t* sim, const std::string& name, race_type_e r = RACE_NONE );
  static player_t* create_mage        ( sim_t* sim, const std::string& name, race_type_e r = RACE_NONE );
  static player_t* create_monk        ( sim_t* sim, const std::string& name, race_type_e r = RACE_NONE );
  static player_t* create_paladin     ( sim_t* sim, const std::string& name, race_type_e r = RACE_NONE );
  static player_t* create_priest      ( sim_t* sim, const std::string& name, race_type_e r = RACE_NONE );
  static player_t* create_rogue       ( sim_t* sim, const std::string& name, race_type_e r = RACE_NONE );
  static player_t* create_shaman      ( sim_t* sim, const std::string& name, race_type_e r = RACE_NONE );
  static player_t* create_warlock     ( sim_t* sim, const std::string& name, race_type_e r = RACE_NONE );
  static player_t* create_warrior     ( sim_t* sim, const std::string& name, race_type_e r = RACE_NONE );
  static player_t* create_enemy       ( sim_t* sim, const std::string& name, race_type_e r = RACE_NONE );

  // Raid-wide aura/buff/debuff maintenance
  static bool init        ( sim_t* sim );
  static void combat_begin( sim_t* sim );
  static void combat_end  ( sim_t* sim );

  // Raid-wide Death Knight buff maintenance
  static void death_knight_init        ( sim_t* sim );
  static void death_knight_combat_begin( sim_t* sim );
  static void death_knight_combat_end  ( sim_t* /* sim */ ) {}

  // Raid-wide Druid buff maintenance
  static void druid_init        ( sim_t* sim );
  static void druid_combat_begin( sim_t* sim );
  static void druid_combat_end  ( sim_t* /* sim */ ) {}

  // Raid-wide Hunter buff maintenance
  static void hunter_init        ( sim_t* sim );
  static void hunter_combat_begin( sim_t* sim );
  static void hunter_combat_end  ( sim_t* /* sim */ ) {}

  // Raid-wide Mage buff maintenance
  static void mage_init        ( sim_t* sim );
  static void mage_combat_begin( sim_t* sim );
  static void mage_combat_end  ( sim_t* /* sim */ ) {}

  // Raid-wide Monk buff maintenance
  static void monk_init        ( sim_t* sim );
  static void monk_combat_begin( sim_t* sim );
  static void monk_combat_end  ( sim_t* /* sim */ ) {}

  // Raid-wide Paladin buff maintenance
  static void paladin_init        ( sim_t* sim );
  static void paladin_combat_begin( sim_t* sim );
  static void paladin_combat_end  ( sim_t* /* sim */ ) {}

  // Raid-wide Priest buff maintenance
  static void priest_init        ( sim_t* sim );
  static void priest_combat_begin( sim_t* sim );
  static void priest_combat_end  ( sim_t* /* sim */ ) {}

  // Raid-wide Rogue buff maintenance
  static void rogue_init        ( sim_t* sim );
  static void rogue_combat_begin( sim_t* sim );
  static void rogue_combat_end  ( sim_t* /* sim */ ) {}

  // Raid-wide Shaman buff maintenance
  static void shaman_init        ( sim_t* sim );
  static void shaman_combat_begin( sim_t* sim );
  static void shaman_combat_end  ( sim_t* /* sim */ ) {}

  // Raid-wide Warlock buff maintenance
  static void warlock_init        ( sim_t* sim );
  static void warlock_combat_begin( sim_t* sim );
  static void warlock_combat_end  ( sim_t* /* sim */ ) {}

  // Raid-wide Warrior buff maintenance
  static void warrior_init        ( sim_t* sim );
  static void warrior_combat_begin( sim_t* sim );
  static void warrior_combat_end  ( sim_t* /* sim */ ) {}

  // Raid-wide Enemy buff maintenance
  static void enemy_init        ( sim_t* sim );
  static void enemy_combat_begin( sim_t* sim );
  static void enemy_combat_end  ( sim_t* /* sim */ ) {}

  inline bool is_pet() const { return type == PLAYER_PET || type == PLAYER_GUARDIAN || type == ENEMY_ADD; }
  inline bool is_enemy() const { return type == ENEMY || type == ENEMY_ADD; }
  inline bool is_add() const { return type == ENEMY_ADD; }

  death_knight_t* cast_death_knight() { assert( type == DEATH_KNIGHT ); return ( death_knight_t* ) this; }
  druid_t       * cast_druid       () { assert( type == DRUID        ); return ( druid_t       * ) this; }
  hunter_t      * cast_hunter      () { assert( type == HUNTER       ); return ( hunter_t      * ) this; }
  mage_t        * cast_mage        () { assert( type == MAGE         ); return ( mage_t        * ) this; }
  monk_t        * cast_monk        () { assert( type == MONK         ); return ( monk_t        * ) this; }
  paladin_t     * cast_paladin     () { assert( type == PALADIN      ); return ( paladin_t     * ) this; }
  rogue_t       * cast_rogue       () { assert( type == ROGUE        ); return ( rogue_t       * ) this; }
  shaman_t      * cast_shaman      () { assert( type == SHAMAN       ); return ( shaman_t      * ) this; }
  warlock_t     * cast_warlock     () { assert( type == WARLOCK      ); return ( warlock_t     * ) this; }
  warrior_t     * cast_warrior     () { assert( type == WARRIOR      ); return ( warrior_t     * ) this; }
  pet_t         * cast_pet         () { assert( is_pet()             ); return ( pet_t         * ) this; }
  enemy_t       * cast_enemy       () { assert( type == ENEMY        ); return ( enemy_t       * ) this; }

  bool      in_gcd() const { return gcd_ready > sim -> current_time; }
  bool      recent_cast() const;
  item_t*   find_item( const std::string& );
  action_t* find_action( const std::string& );
  bool      dual_wield() const { return main_hand_weapon.type != WEAPON_NONE && off_hand_weapon.type != WEAPON_NONE; }

  cooldown_t* find_cooldown( const std::string& name ) const;
  dot_t*      find_dot     ( const std::string& name ) const;
  action_priority_list_t* find_action_priority_list( const std::string& name ) const;
  stats_t*    find_stats   ( const std::string& name );

  cooldown_t* get_cooldown( const std::string& name );
  dot_t*      get_dot     ( const std::string& name );
  gain_t*     get_gain    ( const std::string& name );
  proc_t*     get_proc    ( const std::string& name );
  stats_t*    get_stats   ( const std::string& name, action_t* action=0 );
  benefit_t*  get_benefit ( const std::string& name );
  uptime_t*   get_uptime  ( const std::string& name );
  rng_t*      get_rng     ( const std::string& name );
  double      get_player_distance( const player_t* p ) const;
  double      get_position_distance( double m=0, double v=0 ) const;
  action_priority_list_t* get_action_priority_list( const std::string& name );

  // Opportunity to perform any stat fixups before analysis
  virtual void pre_analyze_hook() {}

  /* New stuff */
  virtual double composite_player_vulnerability( school_type_e ) const;
  virtual double composite_spell_crit_vulnerability() const;
  virtual double composite_attack_crit_vulnerability() const;
  virtual double composite_ranged_attack_player_vulnerability() const;
};

struct compare_scale_factors
{
  const player_t* player;

  compare_scale_factors( const player_t* p ) : player( p ) {}

  bool operator()( const stat_type_e& l, const stat_type_e& r ) const
  {
    return player -> scaling.get_stat( l ) >
           player -> scaling.get_stat( r );
  }
};

struct targetdata_t : public noncopyable
{
  player_t* const source;
  player_t* const target;

  std::vector<dot_t*> dot_list;

  targetdata_t( player_t* source, player_t* target );

  virtual ~targetdata_t();
  virtual void reset();
  virtual void clear_debuffs();

  static targetdata_t* get( player_t* source, player_t* target );

  death_knight_targetdata_t* cast_death_knight() { assert( source->type == DEATH_KNIGHT ); return ( death_knight_targetdata_t* ) this; }
  druid_targetdata_t       * cast_druid       () { assert( source->type == DRUID        ); return ( druid_targetdata_t       * ) this; }
  hunter_targetdata_t      * cast_hunter      () { assert( source->type == HUNTER       ); return ( hunter_targetdata_t      * ) this; }
  mage_targetdata_t        * cast_mage        () { assert( source->type == MAGE         ); return ( mage_targetdata_t        * ) this; }
  monk_targetdata_t        * cast_monk        () { assert( source->type == MONK         ); return ( monk_targetdata_t        * ) this; }
  paladin_targetdata_t     * cast_paladin     () { assert( source->type == PALADIN      ); return ( paladin_targetdata_t     * ) this; }
  priest_targetdata_t      * cast_priest      () { assert( source->type == PRIEST       ); return ( priest_targetdata_t      * ) this; }
  rogue_targetdata_t       * cast_rogue       () { assert( source->type == ROGUE        ); return ( rogue_targetdata_t       * ) this; }
  shaman_targetdata_t      * cast_shaman      () { assert( source->type == SHAMAN       ); return ( shaman_targetdata_t      * ) this; }
  warlock_targetdata_t     * cast_warlock     () { assert( source->type == WARLOCK      ); return ( warlock_targetdata_t     * ) this; }
  warrior_targetdata_t     * cast_warrior     () { assert( source->type == WARRIOR      ); return ( warrior_targetdata_t     * ) this; }

protected:
  dot_t* add_dot( dot_t* d );
  aura_t* add_aura( aura_t* b );
};

// Pet ======================================================================

struct pet_t : public player_t
{
  std::string full_name_str;
  player_t* owner;
  double stamina_per_owner;
  double intellect_per_owner;
  bool summoned;
  pet_type_e pet_type;
  event_t* expiration;

private:
  void init_pet_t_();
public:
  pet_t( sim_t* sim, player_t* owner, const std::string& name, bool guardian=false );
  pet_t( sim_t* sim, player_t* owner, const std::string& name, pet_type_e pt, bool guardian=false );

  // Pets gain their owners' hit rating, but it rounds down to a
  // percentage.  Also, heroic presence does not contribute to pet
  // expertise, so we use raw attack_hit.
  virtual double composite_attack_expertise( const weapon_t* ) const { return floor( floor( 100.0 * owner -> attack_hit ) * ( 26.0 / 8.0 ) ) / 100.0; }
  virtual double composite_attack_hit() const { return floor( 100.0 * owner -> composite_attack_hit() ) / 100.0; }
  virtual double composite_spell_hit() const { return floor( 100.0 * owner -> composite_spell_hit() ) / 100.0;  }
  virtual double composite_player_multiplier( school_type_e school, const action_t* a ) const;

  virtual double composite_attribute( attribute_type_e attr ) const;

  virtual void init_base();
  virtual void init_talents();
  virtual void init_target();
  virtual void reset();
  virtual void summon( timespan_t duration=timespan_t::zero() );
  virtual void dismiss();
  virtual bool ooc_buffs() { return false; }
  virtual double assess_damage( double amount, school_type_e, dmg_type_e, result_type_e, action_t* a=0 );
  virtual void combat_begin();

  virtual const char* name() const { return full_name_str.c_str(); }

  virtual const spell_data_t* find_pet_spell( const std::string& name, const std::string& token = std::string() );
};

// Gain =====================================================================

struct gain_t
{
  std::array<double, RESOURCE_MAX> actual, overflow, count;

  const std::string name_str;
  gain_t* next;

  gain_t( const std::string& n ) :
    name_str( n ), next( NULL )
  {
    range::fill( actual, 0.0 );
    range::fill( overflow, 0.0 );
    range::fill( count, 0.0 );
  }
  void add( const resource_type_e resource_type, const double a, const double o=0 )
  { actual[ resource_type ] += a; overflow[ resource_type ] += o; count[ resource_type ]++; }
  void merge( const gain_t& other )
  {
    for ( size_t i=0; i<RESOURCE_MAX; i++ )
    { actual[i] += other.actual[i]; overflow[i] += other.overflow[i]; count[i] += other.count[i]; }
  }
  void analyze( const sim_t* sim )
  {
    for ( size_t i=0; i<RESOURCE_MAX; i++ )
    { actual[i] /= sim -> iterations; overflow[i] /= sim -> iterations; count[i] /= sim -> iterations; }
  }
  const char* name() const { return name_str.c_str(); }
};

// Stats ====================================================================

struct stats_t
{
  const std::string name_str;
  sim_t* const sim;
  player_t* const player;
  stats_t* parent;
  // We should make school and type const or const-like, and either stricly define when, where and who defines the values,
  // or make sure that it is equal to the value of all it's actions.
  school_type_e school;
  stats_type_e type;

  std::vector<action_t*> action_list;
  gain_t resource_gain;
  // Flags
  bool analyzed;
  bool quiet;
  bool background;

  // Variables used both during combat and for reporting
  double num_executes, num_ticks;
  double num_direct_results, num_tick_results;
  timespan_t total_execute_time, total_tick_time;
  double portion_amount;
  sample_data_t total_intervals;
  timespan_t last_execute;
  double iteration_actual_amount, iteration_total_amount;
  sample_data_t actual_amount, total_amount, portion_aps;
  std::vector<stats_t*> children;
  double compound_actual;
  double opportunity_cost;

  struct stats_results_t
  {
    sample_data_t actual_amount, total_amount,fight_actual_amount, fight_total_amount,count,avg_actual_amount;
    int iteration_count;
    double iteration_actual_amount, iteration_total_amount,pct, overkill_pct;

    stats_results_t( sim_t* );
    void analyze( const stats_t& );
    void merge( const stats_results_t& other );
    void combat_end();
  };
  std::vector<stats_results_t> direct_results;
  std::vector<stats_results_t>   tick_results;

  std::vector<double> timeline_amount;

  // Reporting only
  std::array<double,RESOURCE_MAX> resource_portion, apr, rpe;
  double rpe_sum, compound_amount, overkill_pct;
  double aps, ape, apet, etpe, ttpt;
  timespan_t total_time;
  std::string aps_distribution_chart;

  std::vector<double> timeline_aps;
  std::string timeline_aps_chart;

  // Scale factor container
  gear_stats_t scaling;
  gear_stats_t scaling_error;

  stats_t( const std::string& name, player_t* );

  void add_child( stats_t* child );
  void consume_resource( const resource_type_e resource_type, const double resource_amount );
  void add_result( const double act_amount, const double tot_amount, const dmg_type_e dmg_type, const result_type_e result );
  void add_tick   ( timespan_t time );
  void add_execute( timespan_t time );
  void combat_begin();
  void combat_end();
  void reset();
  void analyze();
  void merge( const stats_t* other );
};

// Action ===================================================================

struct action_t
{
  const spell_data_t* s_data;
  sim_t* const sim;
  const action_type_e type;
  std::string name_str;
  player_t* const player;
  player_t* target;
  uint32_t id;
  result_type_e result;
  school_type_e school;
  int aoe;
  bool dual, callbacks, special, channeled, background, sequence, use_off_gcd;
  bool direct_tick, repeating, harmful, proc, item_proc, proc_ignores_slot, may_trigger_dtr, discharge_proc, auto_cast, initialized;
  bool may_hit, may_miss, may_dodge, may_parry, may_glance, may_block, may_crush, may_crit;
  bool tick_may_crit, tick_zero, hasted_ticks;
  bool no_buffs, no_debuffs;
  bool stateless;
  dot_behaviour_type_e dot_behavior;
  timespan_t ability_lag, ability_lag_stddev;
  double rp_gain;
  timespan_t min_gcd, trigger_gcd;
  double range;
  double weapon_power_mod, direct_power_mod, tick_power_mod;
  timespan_t base_execute_time;
  timespan_t base_tick_time;
  std::array< double, RESOURCE_MAX > base_costs;
  double base_dd_min, base_dd_max, base_td, base_td_init;
  double   base_dd_multiplier,   base_td_multiplier;
  double player_dd_multiplier, player_td_multiplier;
  double   base_multiplier,   base_hit,   base_crit;
  double player_multiplier, player_hit, player_crit;
  double target_multiplier, target_hit, target_crit;
  double   base_spell_power,   base_attack_power;
  double player_spell_power, player_attack_power;
  double target_spell_power, target_attack_power;
  double   base_spell_power_multiplier,   base_attack_power_multiplier;
  double player_spell_power_multiplier, player_attack_power_multiplier;
  double crit_multiplier, crit_bonus_multiplier, crit_bonus;
  double base_dd_adder, player_dd_adder, target_dd_adder;
  double player_haste;
  double resource_consumed;
  double direct_dmg, tick_dmg;
  double snapshot_crit, snapshot_haste, snapshot_mastery;
  int num_ticks;
  weapon_t* weapon;
  double weapon_multiplier;
  double base_add_multiplier;
  double base_aoe_multiplier; // Static reduction of damage for AoE
  bool normalize_weapon_speed;
  rng_t* rng_result;
  rng_t* rng_travel;
  cooldown_t* cooldown;
  mutable dot_t* action_dot;
  stats_t* stats;
  event_t* execute_event;
  event_t* travel_event;
  timespan_t time_to_execute, time_to_travel;
  double travel_speed;
  int bloodlust_active;
  double min_health_percentage, max_health_percentage;
  int moving, wait_on_ready, interrupt;
  bool round_base_dmg;
  bool class_flag1;
  std::string if_expr_str;
  expr_t* if_expr;
  std::string interrupt_if_expr_str;
  expr_t* interrupt_if_expr;
  std::string sync_str;
  action_t* sync_action;
  char marker;
  std::string signature_str;
  std::string target_str;
  std::string label_str;
  timespan_t last_reaction_time;
  action_t* dtr_action;
  bool is_dtr_action;
  bool can_trigger_dtr;
  int targetdata_dot_offset;

private:
  mutable player_t* cached_targetdata_target;
  mutable targetdata_t* cached_targetdata;

public:
  action_t( action_type_e type, const std::string& token, player_t* p, const spell_data_t* s = spell_data_t::nil(), school_type_e school = SCHOOL_NONE );
  virtual ~action_t();
  void init_dot( const std::string& dot_name );

  const spell_data_t& data() const { return ( *s_data ); }
  virtual void   parse_spell_data( const spell_data_t& );
  virtual void   parse_effect_data( const spelleffect_data_t& );

  virtual void   parse_options( option_t*, const std::string& options_str );
  virtual double cost() const;
  virtual double total_haste() const  { return haste();           }
  virtual double haste() const        { return 1.0;               }
  virtual timespan_t gcd() const;
  virtual timespan_t execute_time() const { return base_execute_time; }
  virtual timespan_t tick_time( double haste ) const;
  virtual int    hasted_num_ticks( double haste, timespan_t d=timespan_t::min() ) const;
  virtual timespan_t travel_time() const;
  virtual void   player_buff();
  virtual void   player_tick() {}
  virtual void   target_debuff( player_t* t, dmg_type_e );
  virtual void   snapshot();
  virtual result_type_e calculate_result( double /* crit */, unsigned /* target_level */ ) { assert( false ); return RESULT_UNKNOWN; }
  virtual bool   result_is_hit ( result_type_e = RESULT_UNKNOWN ) const;
  virtual bool   result_is_miss( result_type_e = RESULT_UNKNOWN ) const;
  virtual double calculate_direct_damage( result_type_e, int chain_target, unsigned target_level,
                                          double attack_power,  double spell_power, double multiplier );
  virtual double calculate_tick_damage( result_type_e, double power, double multiplier );
  virtual double calculate_weapon_damage( double attack_power );
  virtual double armor() const;
  virtual double resistance() const;
  virtual void   consume_resource();
  virtual resource_type_e current_resource() const
  {
    if ( likely( s_data && s_data -> _power && s_data -> _power -> size() == 1 ) )
      return s_data -> _power -> at( 0 ) -> resource();

    return RESOURCE_NONE;
  }
  virtual void   execute();
  virtual void   tick( dot_t* d );
  virtual void   last_tick( dot_t* d );
  virtual void   impact( player_t*, result_type_e result, double dmg );
  virtual void   assess_damage( player_t* t, double amount, dmg_type_e, result_type_e impact_result );
  virtual void   additional_damage( player_t* t, double amount, dmg_type_e, result_type_e impact_result );
  virtual void   schedule_execute();
  virtual void   schedule_travel( player_t* t );
  virtual void   reschedule_execute( timespan_t time );
  virtual void   update_ready();
  virtual bool   usable_moving();
  virtual bool   ready();
  virtual void   init();
  virtual void   reset();
  virtual void   cancel();
  virtual void   interrupt_action();
  virtual void   check_talent( int talent_rank );
  virtual void   check_spec( specialization_e necessary_spec );
  virtual void   check_race( race_type_e race );
  virtual void   check_spell( const spell_data_t* sp );
  virtual const char* name() const { return name_str.c_str(); }

  virtual double   miss_chance( double /* hit */, int /* delta_level */ ) const { return 0; }
  virtual double  dodge_chance( double /* expertise */, int /* delta_level */ ) const { return 0; }
  virtual double  parry_chance( double /* expertise */, int /* delta_level */ ) const { return 0; }
  virtual double glance_chance( int /* delta_level */ ) const { return 0; }
  virtual double  block_chance( int /* delta_level */ ) const { return 0; }
  virtual double   crit_chance( double /* crit */, int /* delta_level */ ) const { return 0; }

  virtual double total_multiplier() const { return   base_multiplier * player_multiplier * target_multiplier; }
  virtual double total_crit() const       { return   base_crit       + player_crit       + target_crit;       }
  virtual double total_crit_bonus() const;

  virtual double total_spell_power() const  { return floor( ( base_spell_power  + player_spell_power  + target_spell_power  ) * base_spell_power_multiplier  * player_spell_power_multiplier  ); }
  virtual double total_attack_power() const { return floor( ( base_attack_power + player_attack_power + target_attack_power ) * base_attack_power_multiplier * player_attack_power_multiplier ); }
  virtual double total_power() const;

  // Some actions require different multipliers for the "direct" and "tick" portions.

  virtual double total_dd_multiplier() const { return total_multiplier() * base_dd_multiplier * player_dd_multiplier; }
  virtual double total_td_multiplier() const { return total_multiplier() * base_td_multiplier * player_td_multiplier; }

  virtual double bonus_damage() const { return base_dd_adder; }

  virtual expr_t* create_expression( const std::string& name );

  virtual double ppm_proc_chance( double PPM ) const;

  dot_t* dot() const
  {
    if ( targetdata_dot_offset >= 0 )
      return *( dot_t** )( ( char* )targetdata() + targetdata_dot_offset );
    else
    {
      if ( ! action_dot )
        action_dot = player -> get_dot( name_str );
      return action_dot;
    }
  }

  void add_child( action_t* child ) { stats -> add_child( child -> stats ); }

  // Move to ability_t in future
  const spelleffect_data_t& effect1() const { return data().effect1(); }
  const spelleffect_data_t& effect2() const { return data().effect2(); }
  const spelleffect_data_t& effect3() const { return data().effect3(); }

  targetdata_t* targetdata() const
  {
    if ( cached_targetdata_target != target )
    {
      cached_targetdata_target = target;
      cached_targetdata = targetdata_t::get( player, target );
    }
    return cached_targetdata;
  }

  virtual size_t available_targets( std::vector< player_t* >& ) const;
  virtual std::vector< player_t* > target_list() const;

  /* New stuff */
  action_state_t* state_cache;
  action_state_t* execute_state; /* State of the last execute() */
  uint32_t snapshot_flags;
  uint32_t update_flags;

  virtual action_state_t* new_state();
  virtual action_state_t* get_state( const action_state_t* = 0 );
  virtual void release_state( action_state_t* );
  virtual void schedule_travel_s( action_state_t* );
  virtual void impact_s( action_state_t* );

  virtual void   snapshot_state( action_state_t*, uint32_t );

  virtual double action_multiplier( const action_state_t* ) const { return base_multiplier; }
  virtual double action_da_multiplier() const { return base_dd_multiplier; }
  virtual double action_ta_multiplier() const { return base_td_multiplier; }

  virtual double composite_hit() const { return base_hit; }
  virtual double composite_expertise() const { return 0.0; }
  virtual double composite_crit( const action_state_t* ) const { return base_crit; }
  virtual double composite_haste() const { return 1.0; }
  virtual double composite_attack_power() const { return base_attack_power; }
  virtual double composite_attack_power_multiplier() const { return base_attack_power_multiplier; }
  virtual double composite_spell_power() const { return base_spell_power; }
  virtual double composite_spell_power_multiplier() const { return base_spell_power_multiplier; }
  virtual double composite_da_multiplier( const action_state_t* s ) const
  {
    return action_multiplier( s ) * action_da_multiplier() *
           player -> composite_player_multiplier( school, this ) *
           player -> composite_player_dd_multiplier( school, this );
  }
  virtual double composite_ta_multiplier( const action_state_t* s ) const
  {
    return action_multiplier( s ) * action_ta_multiplier() *
           player -> composite_player_multiplier( school, this ) *
           player -> composite_player_td_multiplier( school, this );
  }
};

struct action_state_t
{
  // Source action, target actor
  action_t*       action;
  player_t*       target;
  // Results
  result_type_e   result;
  double          result_amount;
  // Snapshotted stats during execution
  double          haste;
  double          crit;
  double          attack_power;
  double          spell_power;
  // Multipliers
  double          da_multiplier;
  double          ta_multiplier;
  double          target_multiplier;
  // Cache pointer
  action_state_t* next;

  action_state_t( action_t*, player_t* );
  virtual ~action_state_t() {};

  virtual void copy_state( const action_state_t* );
  virtual void debug() const;

  virtual inline double composite_power() const
  {
    return attack_power + spell_power;
  }

  virtual inline double composite_da_multiplier() const
  {
    return da_multiplier * target_multiplier;
  }

  virtual inline double composite_ta_multiplier() const
  {
    return ta_multiplier * target_multiplier;
  }
};

// Attack ===================================================================

struct attack_t : public action_t
{
public:
  attack_t( const std::string& token, player_t* p, const spell_data_t* s = spell_data_t::nil(), school_type_e sc=SCHOOL_PHYSICAL );

  // Attack Overrides
  virtual double haste() const;
  virtual double total_haste() const { return swing_haste(); }
  virtual double swing_haste() const;
  virtual timespan_t execute_time() const;
  virtual void   player_buff();
  int build_table( std::array<double,RESULT_MAX>& chances,
                   std::array<result_type_e,RESULT_MAX>& results,
                   unsigned level,
                   double /* attack_crit */ );
  virtual result_type_e calculate_result( double, unsigned );
  virtual void   init();

  virtual double   miss_chance( double hit, int delta_level ) const;
  virtual double  block_chance( int /* delta_level */ ) const { return 0.0; }
  virtual double  crit_block_chance( int delta_level ) const;
  virtual double   crit_chance( double crit, int delta_level ) const;

  /* New stuffs */
  virtual double composite_hit() const
  { return action_t::composite_hit() + player -> composite_attack_hit(); }
  virtual double composite_crit( const action_state_t* s ) const
  { return action_t::composite_crit( s ) + player -> composite_attack_crit( weapon ); }
  virtual double composite_haste() const
  { return action_t::composite_haste() * player -> composite_attack_haste(); }
  virtual double composite_attack_power() const { return action_t::composite_attack_power() + player -> composite_attack_power(); }
  virtual double composite_attack_power_multiplier() const { return action_t::composite_attack_power_multiplier() * player -> composite_attack_power_multiplier(); }
};

// Melee Attack ===================================================================

struct melee_attack_t : public attack_t
{
  double base_expertise, player_expertise, target_expertise;

public:
  melee_attack_t( const std::string& token, player_t* p, const spell_data_t* s = spell_data_t::nil(), school_type_e sc=SCHOOL_PHYSICAL );

  // Melee Attack Overrides
  virtual void   player_buff();
  virtual void   target_debuff( player_t* t, dmg_type_e );
  virtual double total_expertise() const;

  virtual double  dodge_chance( double /* expertise */, int delta_level ) const;
  virtual double  parry_chance( double /* expertise */, int delta_level ) const;
  virtual double glance_chance( int delta_level ) const;

  /* New stuffs */
  virtual double composite_expertise() const { return attack_t::composite_expertise() + player -> composite_attack_expertise( weapon ); }
};

// Ranged Attack ===================================================================

struct ranged_attack_t : public attack_t
{
  double base_expertise, player_expertise, target_expertise;

public:
  ranged_attack_t( const std::string& token, player_t* p, const spell_data_t* s = spell_data_t::nil(), school_type_e sc=SCHOOL_PHYSICAL );

  // Ranged Attack Overrides
  virtual void   player_buff();
  virtual void   target_debuff( player_t* t, dmg_type_e );
  virtual double total_expertise() const;

  virtual double  dodge_chance( double /* expertise */, int delta_level ) const;
  virtual double  parry_chance( double /* expertise */, int delta_level ) const;
  virtual double glance_chance( int delta_level ) const;

  /* New stuffs */
  virtual double composite_expertise() const { return attack_t::composite_expertise() + player -> composite_attack_expertise( weapon ); }
};

// Spell Base ====================================================================

struct spell_base_t : public action_t
{
public:
  spell_base_t( action_type_e at, const std::string& token, player_t* p, const spell_data_t* s = spell_data_t::nil(), school_type_e sc=SCHOOL_PHYSICAL );
  // Spell Overrides
  virtual double haste() const;
  virtual timespan_t gcd() const;
  virtual timespan_t execute_time() const;
  virtual void   player_buff();
  virtual result_type_e   calculate_result( double, unsigned );
  virtual void   execute();
  virtual double crit_chance( double crit, int delta_level ) const;
  virtual void   schedule_execute();
  virtual void   init();

  /* New stuffs */
  virtual double composite_crit( const action_state_t* s ) const
  { return action_t::composite_crit( s ) + player -> composite_spell_crit(); }
  virtual double composite_haste() const { return action_t::composite_haste() * player -> composite_spell_haste(); }
  virtual double composite_spell_power() const { return action_t::composite_spell_power() + player -> composite_spell_power( school ); }
  virtual double composite_spell_power_multiplier() const { return action_t::composite_spell_power_multiplier() * player -> composite_spell_power_multiplier(); }
};

// Harmful Spell ====================================================================

struct spell_t : public spell_base_t
{
public:
  spell_t( const std::string& token, player_t* p, const spell_data_t* s = spell_data_t::nil(), school_type_e sc = SCHOOL_NONE );

  // Harmful Spell Overrides
  virtual void   player_buff();
  virtual void   target_debuff( player_t* t, dmg_type_e );
  virtual void   execute();
  virtual double miss_chance( double hit, int delta_level ) const;

  /* New stuffs */
  virtual double composite_hit() const { return action_t::composite_hit() + player -> composite_spell_hit(); }
};

// Heal =====================================================================

struct heal_t : public spell_base_t
{
  bool group_only;
  // Reporting
  double total_heal, total_actual;

public:
  heal_t( const std::string& name, player_t* p, const spell_data_t* s = spell_data_t::nil(), school_type_e sc=SCHOOL_NONE );

  virtual void player_buff();
  virtual void execute();
  virtual void assess_damage( player_t* t, double amount, dmg_type_e, result_type_e );
  player_t* find_greatest_difference_player();
  player_t* find_lowest_player();
  virtual size_t available_targets( std::vector< player_t* >& ) const;
};

// Absorb ===================================================================

struct absorb_t : public spell_base_t
{
  // Reporting
  double total_heal, total_actual;

public:
  absorb_t( const std::string& name, player_t* p, const spell_data_t* s = spell_data_t::nil(), school_type_e sc=SCHOOL_NONE );

  virtual void player_buff();
  virtual void execute();
  virtual void assess_damage( player_t* t, double amount,
                              dmg_type_e, result_type_e impact_result );
  virtual void impact( player_t*, result_type_e impact_result, double travel_dmg );

  /* New stuffs */
  virtual void impact_s( action_state_t* );
};

// Sequence =================================================================

struct sequence_t : public action_t
{
  std::vector<action_t*> sub_actions;
  int current_action;
  bool restarted;
  timespan_t last_restart;

  sequence_t( player_t*, const std::string& sub_action_str );

  virtual void schedule_execute();
  virtual void reset();
  virtual bool ready();
  void restart() { current_action = 0; restarted = true; last_restart = sim -> current_time; }
  bool can_restart()
  { return ! restarted || last_restart + timespan_t::from_millis( 1 ) < sim -> current_time; }
};

// Cooldown =================================================================

struct cooldown_t
{
  sim_t* const sim;
  player_t* const player;
  const std::string name_str;
  timespan_t duration;
  timespan_t ready;
  cooldown_t* next;

  cooldown_t( const std::string& n, player_t* p ) : sim( p->sim ), player( p ), name_str( n ), duration( timespan_t::zero() ), ready( ready_init() ), next( 0 ) {}
  cooldown_t( const std::string& n, sim_t* s ) : sim( s ), player( 0 ), name_str( n ), duration( timespan_t::zero() ), ready( ready_init() ), next( 0 ) {}

  void reset() { ready=ready_init(); }
  void start( timespan_t override=timespan_t::min(), timespan_t delay=timespan_t::zero() )
  {
    if ( override >= timespan_t::zero() ) duration = override;
    if ( duration > timespan_t::zero() ) ready = sim -> current_time + duration + delay;
  }
  timespan_t remains() const
  {
    timespan_t diff = ready - sim -> current_time;
    if ( diff < timespan_t::zero() ) diff = timespan_t::zero();
    return diff;
  }
  const char* name() const { return name_str.c_str(); }

private:
  static timespan_t ready_init()
  { return timespan_t::from_seconds( -60 * 60 ); }
};

// DoT ======================================================================

struct dot_t
{
  sim_t* const sim;
  player_t* const player;
  action_t* action;
  event_t* tick_event;
  int num_ticks, current_tick, added_ticks, ticking;
  timespan_t added_seconds;
  timespan_t ready;
  timespan_t miss_time;
  timespan_t time_to_tick;
  const std::string name_str;
  double prev_tick_amount;
  /* New stuff */
  action_state_t* state;

  dot_t( const std::string& n, player_t* p );

  void   cancel();
  void   extend_duration( int extra_ticks, bool cap=false );
  void   extend_duration_seconds( timespan_t extra_seconds );
  void   recalculate_ready();
  void   refresh_duration();
  void   reset();
  timespan_t remains() const;
  void   schedule_tick();
  int    ticks() const;
  expr_t* create_expression( const std::string& name_str );

  const char* name() const { return name_str.c_str(); }
  /* New stuff */
  void schedule_new_tick();
};

// Action Callback ==========================================================

struct action_callback_t
{
  player_t* listener;
  bool active;
  bool allow_self_procs;
  bool allow_item_procs;
  bool allow_procs;

  action_callback_t( player_t* l, bool ap=false, bool aip=false, bool asp=false ) :
    listener( l ), active( true ), allow_self_procs( asp ), allow_item_procs( aip ), allow_procs( ap )
  {
    assert( l );
    l -> all_callbacks.push_back( this );
  }
  virtual ~action_callback_t() {}
  virtual void trigger( action_t*, void* call_data=0 ) = 0;
  virtual void reset() {}
  virtual void activate() { active=true; }
  virtual void deactivate() { active=false; }

  static void trigger( const std::vector<action_callback_t*>& v, action_t* a, void* call_data=0 )
  {
    if ( a && ! a -> player -> in_combat ) return;

    std::size_t size = v.size();
    for ( std::size_t i=0; i < size; i++ )
    {
      action_callback_t* cb = v[ i ];
      if ( cb -> active )
      {
        if ( ! cb -> allow_item_procs && a && a -> item_proc ) return;
        if ( ! cb -> allow_procs && a && a -> proc ) return;
        cb -> trigger( a, call_data );
      }
    }
  }

  static void reset( const std::vector<action_callback_t*>& v )
  {
    std::size_t size = v.size();
    for ( std::size_t i=0; i < size; i++ )
    {
      v[ i ] -> reset();
    }
  }
};

// Action Priority List =====================================================

struct action_priority_list_t
{
  const std::string name_str;
  std::string action_list_str;
  player_t* const player;
  action_priority_list_t( std::string name, player_t* p ) : name_str( name ), player( p )
  {}
};

// Player Ready Event =======================================================

struct player_ready_event_t : public event_t
{
  player_ready_event_t( sim_t* sim, player_t* p, timespan_t delta_time );
  virtual void execute();
};

struct player_gcd_event_t : public event_t
{
  player_gcd_event_t( sim_t* sim, player_t* p, timespan_t delta_time );
  virtual void execute();
};

// Action Execute Event =====================================================

struct action_execute_event_t : public event_t
{
  action_t* const action;
  action_execute_event_t( sim_t* sim, action_t* a, timespan_t time_to_execute );
  virtual void execute();
};

// DoT Tick Event ===========================================================

struct dot_tick_event_t : public event_t
{
  dot_t* const dot;
  dot_tick_event_t( sim_t* sim, dot_t* d, timespan_t time_to_tick );
  virtual void execute();
};

// Action Travel Event ======================================================

struct action_travel_event_t : public event_t
{
  action_t* const action;
  player_t* const target;
  result_type_e result;
  double damage;
  action_travel_event_t( sim_t* sim, player_t* t, action_t* a, timespan_t time_to_travel );
  virtual void execute();
};

struct stateless_travel_event_t : public event_t
{
  action_t* const action;
  action_state_t* const state;
  stateless_travel_event_t( sim_t* sim, action_t* a, action_state_t* state, timespan_t time_to_travel );
  virtual ~stateless_travel_event_t() { if ( unlikely( state && canceled ) ) action -> release_state( state ); }
  virtual void execute();
};

// Regen Event ==============================================================

struct regen_event_t : public event_t
{
  regen_event_t( sim_t* sim );
  virtual void execute();
};

// Unique Gear ==============================================================

struct unique_gear_t
{
  static void init( player_t* );

  static action_callback_t* register_stat_proc( proc_type_e, int64_t mask, const std::string& name, player_t*,
                                                stat_type_e, int max_stacks, double amount,
                                                double proc_chance, timespan_t duration, timespan_t cooldown,
                                                timespan_t tick=timespan_t::zero(), bool reverse=false );

  static action_callback_t* register_cost_reduction_proc( proc_type_e, int64_t mask, const std::string& name, player_t*,
                                                          school_type_e, int max_stacks, double amount,
                                                          double proc_chance, timespan_t duration, timespan_t cooldown,
                                                          bool refreshes=false, bool reverse=false );

  static action_callback_t* register_discharge_proc( proc_type_e, int64_t mask, const std::string& name, player_t*,
                                                     int max_stacks, school_type_e, double amount, double scaling,
                                                     double proc_chance, timespan_t cooldown, bool no_buffs, bool no_debuffs,
                                                     unsigned int override_result_type_es_mask = 0, unsigned int results_types_mask = 0 );

  static action_callback_t* register_chance_discharge_proc( proc_type_e, int64_t mask, const std::string& name, player_t*,
                                                            int max_stacks, school_type_e, double amount, double scaling,
                                                            double proc_chance, timespan_t cooldown, bool no_buffs, bool no_debuffs,
                                                            unsigned int override_result_type_es_mask = 0, unsigned int results_types_mask = 0 );

  static action_callback_t* register_stat_discharge_proc( proc_type_e, int64_t mask, const std::string& name, player_t*,
                                                          int max_stacks, stat_type_e, double stat_amount,
                                                          school_type_e, double discharge_amount, double discharge_scaling,
                                                          double proc_chance, timespan_t duration, timespan_t cooldown, bool no_buffs, bool no_debuffs,
                                                          unsigned int override_result_type_es_mask = 0, unsigned int results_types_mask = 0 );

  static action_callback_t* register_stat_proc( item_t&, item_t::special_effect_t& );
  static action_callback_t* register_cost_reduction_proc( item_t&, item_t::special_effect_t& );
  static action_callback_t* register_discharge_proc( item_t&, item_t::special_effect_t& );
  static action_callback_t* register_chance_discharge_proc( item_t&, item_t::special_effect_t& );
  static action_callback_t* register_stat_discharge_proc( item_t&, item_t::special_effect_t& );

  static bool get_equip_encoding( std::string& encoding,
                                  const std::string& item_name,
                                  const bool         item_heroic,
                                  const bool         item_lfr,
                                  const bool         ptr,
                                  const std::string& item_id=std::string() );

  static bool get_use_encoding  ( std::string& encoding,
                                  const std::string& item_name,
                                  const bool         heroic,
                                  const bool         lfr,
                                  const bool         ptr,
                                  const std::string& item_id=std::string() );
};

// Enchants =================================================================

struct enchant_t
{
  static void init( player_t* );
  static bool get_encoding        ( std::string& name, std::string& encoding, const std::string& enchant_id, const bool ptr );
  static bool get_addon_encoding  ( std::string& name, std::string& encoding, const std::string& addon_id, const bool ptr );
  static bool get_reforge_encoding( std::string& name, std::string& encoding, const std::string& reforge_id );
  static int  get_reforge_id      ( stat_type_e stat_from, stat_type_e stat_to );
  static bool download        ( item_t&, const std::string& enchant_id );
  static bool download_addon  ( item_t&, const std::string& addon_id   );
  static bool download_reforge( item_t&, const std::string& reforge_id );
  static bool download_rsuffix( item_t&, const std::string& rsuffix_id );
};

// Consumable ===============================================================

struct consumable_t
{
  static action_t* create_action( player_t*, const std::string& name, const std::string& options );
};

// Benefit ==================================================================

struct benefit_t : public noncopyable
{
private:
  int up, down;
public:
  double ratio;
  benefit_t* next;
  const std::string name_str;

  explicit benefit_t( const std::string& n ) :
    up( 0 ), down( 0 ),
    ratio( 0.0 ), name_str( n ) {}

  void update( int is_up ) { if ( is_up ) up++; else down++; }

  const char* name() const { return name_str.c_str(); }

  void analyze()
  {
    if ( up != 0 )
      ratio = 1.0 * up / ( down + up );
  }

  void merge( const benefit_t* other )
  { up += other -> up; down += other -> down; }
};

// Uptime ==================================================================

struct uptime_common_t
{
  timespan_t last_start;
  timespan_t iteration_uptime_sum;
  sample_data_t uptime_sum;
  sim_t* const sim;

  uptime_common_t( sim_t* s ) :
    last_start( timespan_t::min() ), iteration_uptime_sum( timespan_t::zero() ), uptime_sum( s -> statistics_level < 6 ), sim( s )
  {}

  void update( bool is_up )
  {
    if ( is_up )
    {
      if ( last_start < timespan_t::zero() )
        last_start = sim -> current_time;
    }
    else if ( last_start >= timespan_t::zero() )
    {
      iteration_uptime_sum += sim -> current_time - last_start;
      last_start = timespan_t::min();
    }
  }

  void combat_end()
  {
    uptime_sum.add( sim->current_time.total_seconds() ? iteration_uptime_sum.total_seconds() / sim->current_time.total_seconds() : 0.0 );
    iteration_uptime_sum = timespan_t::zero();
  }

  void reset() { last_start = timespan_t::min(); }

  void analyze()
  { uptime_sum.analyze(); }

  void merge( const uptime_common_t& other )
  { uptime_sum.merge( other.uptime_sum ); }
};

struct uptime_t : public uptime_common_t
{
  const std::string name_str;
  uptime_t* next;

  uptime_t( sim_t* s, const std::string& n ) :
    uptime_common_t( s ), name_str( n )
  {}

  const char* name() const { return name_str.c_str(); }
};

struct buff_uptime_t : public uptime_common_t
{ buff_uptime_t( sim_t* s ) : uptime_common_t( s ) {} };

// Proc =====================================================================

struct proc_t
{
  sim_t* const sim;
  player_t* const player;
  const std::string name_str;

  double count;
  timespan_t last_proc;
  sample_data_t interval_sum;
  proc_t* next;

  proc_t( sim_t* s, player_t* p, const std::string& n ) :
    sim( s ), player( p ), name_str( n ),
    count( 0.0 ), last_proc( timespan_t::zero() ), interval_sum( s -> statistics_level < 6 ),
    next( NULL )
  {}

  void occur()
  {
    count++;
    if ( last_proc > timespan_t::zero() && last_proc < sim -> current_time )
    {
      interval_sum.add( sim -> current_time.total_seconds() - last_proc.total_seconds() );
    }
    last_proc = sim -> current_time;
  }

  void merge( const proc_t& other )
  {
    count          += other.count;
    interval_sum.merge( other.interval_sum );
  }

  void analyze()
  {
    count /= sim -> iterations;
    interval_sum.analyze();
  }

  const char* name() const { return name_str.c_str(); }
};

// Report ===================================================================

struct report_t
{
  // In the end, the idea is to only have print_suite remaining, and completly separate the report from simulationcraft.hpp
  static void print_spell_query( sim_t*, unsigned level = MAX_LEVEL );
  static void print_profiles( sim_t* );
  static void print_text( FILE*, sim_t*, bool detail=true );

  static void print_suite( sim_t* );
};

// Log ======================================================================

struct log_t
{
  // Generic Output
  static void output( sim_t*, const char* format, ... ) PRINTF_ATTRIBUTE( 2,3 );

  // Combat Log (unsupported)
};

// Pseudo Random Number Generation ==========================================

struct rng_t
{
public:
  const std::string name_str;
  double expected_roll,  actual_roll,  num_roll;
  double expected_range, actual_range, num_range;
  double expected_gauss, actual_gauss, num_gauss;
  rng_t* next;

private:
  double gauss_pair_value;
  bool   gauss_pair_use;
public:
  bool   average_range, average_gauss;
  // FIXME: Change rng-creation so that average_range and average_gauss can be protected again.
protected:
  rng_t( const std::string& n, bool avg_range=false, bool avg_gauss=false );

public:
  virtual ~rng_t() {}

  virtual rng_type_e type() const = 0;
  virtual double  real() = 0;
  virtual bool    roll( double chance );
  virtual double range( double min, double max );
  virtual double gauss( double mean, double stddev, const bool truncate_low_end = false );
  double exgauss( double mean, double stddev, double nu );
  virtual void    seed( uint32_t start = time( NULL ) );
  void   report( FILE* );

  timespan_t range( timespan_t min, timespan_t max )
  {
    return timespan_t::from_native( range( (double) timespan_t::to_native( min ),
                                           (double) timespan_t::to_native( max ) ) );
  }

  timespan_t gauss( timespan_t mean, timespan_t stddev )
  {
    return timespan_t::from_native( gauss( (double) timespan_t::to_native( mean ),
                                           (double) timespan_t::to_native( stddev ) ) );
  }

  timespan_t exgauss( timespan_t mean, timespan_t stddev, timespan_t nu )
  {
    return timespan_t::from_native( exgauss( (double) timespan_t::to_native( mean ),
                                             (double) timespan_t::to_native( stddev ),
                                             (double) timespan_t::to_native( nu ) ) );
  }

  static double stdnormal_cdf( double u );
  static double stdnormal_inv( double p );

  static rng_t* create( sim_t*, const std::string& name, rng_type_e type=RNG_STANDARD );
};

// String utils =============================================================

std::string tolower( const std::string& src );
std::string proper_option_name( const std::string& full_name );
#if 0 // UNUSED
std::string trim( const std::string& src );
void replace_char( std::string& str, char old_c, char new_c  );
void replace_str( std::string& str, const std::string& old_str, const std::string& new_str  );
bool str_to_float( const std::string& src, double& dest );
#endif // UNUSED

// Armory ===================================================================

struct armory_t
{
  static void fuzzy_stats( std::string& encoding, const std::string& description );
  static meta_gem_type_e parse_meta_gem( const std::string& description );
  static std::string& format( std::string& name, int format_type_e = FORMAT_DEFAULT );
};

// Wowhead  =================================================================

struct wowhead_t
{
  static player_t* download_player( sim_t* sim,
                                    const std::string& region,
                                    const std::string& server,
                                    const std::string& name,
                                    const std::string& spec,
                                    cache::behavior_e b=cache::players() );
  static bool download_slot( item_t&,
                             const std::string& item_id,
                             const std::string& enchant_id,
                             const std::string& addon_id,
                             const std::string& reforge_id,
                             const std::string& rsuffix_id,
                             const std::string gem_ids[ 3 ],
                             bool ptr=false,
                             cache::behavior_e b=cache::items() );
  static bool download_item( item_t&, const std::string& item_id,
                             bool ptr=false, cache::behavior_e b=cache::items() );
  static bool download_glyph( player_t* player, std::string& glyph_name, const std::string& glyph_id,
                              bool ptr=false, cache::behavior_e b=cache::items() );
  static gem_type_e parse_gem( item_t& item, const std::string& gem_id,
                               bool ptr=false, cache::behavior_e b=cache::items() );
};

// CharDev  =================================================================

struct chardev_t
{
  static player_t* download_player( sim_t* sim, const std::string& id, cache::behavior_e b=cache::players() );
};

// MMO Champion =============================================================

struct mmo_champion_t
{
  static bool download_slot( item_t&,
                             const std::string& item_id,
                             const std::string& enchant_id,
                             const std::string& addon_id,
                             const std::string& reforge_id,
                             const std::string& rsuffix_id,
                             const std::string gem_ids[ 3 ],
                             cache::behavior_e b=cache::items() );
  static bool download_item( item_t&, const std::string& item_id,
                             cache::behavior_e b=cache::items() );
  static bool download_glyph( player_t* player, std::string& glyph_name,
                              const std::string& glyph_id, cache::behavior_e b=cache::items() );
  static gem_type_e parse_gem( item_t& item, const std::string& gem_id,
                               cache::behavior_e b=cache::items() );
};

// Rawr =====================================================================

struct rawr_t
{
  static player_t* load_player( sim_t*, const std::string& character_filename );
  static player_t* load_player( sim_t*, const std::string& character_filename, const std::string& character_xml );
};

// Blizzard Community Platform API ==========================================

namespace bcp_api
{
bool download_guild( sim_t* sim,
                     const std::string& region,
                     const std::string& server,
                     const std::string& name,
                     const std::vector<int>& ranks,
                     int player_type_e = PLAYER_NONE,
                     int max_rank=0,
                     cache::behavior_e b=cache::players() );
player_t* download_player( sim_t*,
                           const std::string& region,
                           const std::string& server,
                           const std::string& name,
                           const std::string& talents=std::string( "active" ),
                           cache::behavior_e b=cache::players() );
bool download_item( item_t&, const std::string& item_id, cache::behavior_e b=cache::items() );
bool download_glyph( player_t* player, std::string& glyph_name, const std::string& glyph_id,
                     cache::behavior_e b=cache::items() );
bool download_slot( item_t& item,
                    const std::string& item_id,
                    const std::string& enchant_id,
                    const std::string& addon_id,
                    const std::string& reforge_id,
                    const std::string& rsuffix_id,
                    const std::string gem_ids[ 3 ],
                    cache::behavior_e b=cache::items() );
gem_type_e parse_gem( item_t& item, const std::string& gem_id, cache::behavior_e b=cache::items() );
}

// Wowreforge ===============================================================

namespace wowreforge
{
player_t* download_player( sim_t* sim, const std::string& id, cache::behavior_e b=cache::players() );
};

// HTTP Download  ===========================================================

struct http_t
{
private:
  static void format_( std::string& encoded_url, const std::string& url );
public:
  struct proxy_t
  {
    std::string type;
    std::string host;
    int port;
  };
  static proxy_t proxy;

  static void cache_load();
  static void cache_save();
  static bool clear_cache( sim_t*, const std::string& name, const std::string& value );

  static bool get( std::string& result, const std::string& url, cache::behavior_e b,
                   const std::string& confirmation=std::string() );

  static std::string& format( std::string& encoded_url, const std::string& url )
  { format_( encoded_url, url ); return encoded_url; }
  static std::string& format( std::string& url )
  { format_( url, url ); return url; }
};

// XML ======================================================================

struct xml_t
{
  static const char* get_name( xml_node_t* node );
  static xml_node_t* get_child( xml_node_t* root, const std::string& name );
  static xml_node_t* get_node ( xml_node_t* root, const std::string& path );
  static xml_node_t* get_node ( xml_node_t* root, const std::string& path, const std::string& parm_name, const std::string& parm_value );
  static int  get_children( std::vector<xml_node_t*>&, xml_node_t* root, const std::string& name = std::string() );
  static int  get_nodes   ( std::vector<xml_node_t*>&, xml_node_t* root, const std::string& path );
  static int  get_nodes   ( std::vector<xml_node_t*>&, xml_node_t* root, const std::string& path, const std::string& parm_name, const std::string& parm_value );
  static bool get_value( std::string& value, xml_node_t* root, const std::string& path = std::string() );
  static bool get_value( int&         value, xml_node_t* root, const std::string& path = std::string() );
  static bool get_value( double&      value, xml_node_t* root, const std::string& path = std::string() );
  static xml_node_t* get( sim_t* sim, const std::string& url, cache::behavior_e b,
                          const std::string& confirmation=std::string() );
  static xml_node_t* create( sim_t* sim, const std::string& input );
  static xml_node_t* create( sim_t* sim, FILE* input );
  static void print( xml_node_t* root, FILE* f=0, int spacing=0 );
};

// Java Script ==============================================================

struct js_t
{
  static js_node_t* get_child( js_node_t* root, const std::string& name );
  static js_node_t* get_node ( js_node_t* root, const std::string& path );
  static int  get_children( std::vector<js_node_t*>&, js_node_t* root );
  static int  get_value( std::vector<std::string>& value, js_node_t* root, const std::string& path = std::string() );
  static bool get_value( std::string& value, js_node_t* root, const std::string& path = std::string() );
  static bool get_value( int&         value, js_node_t* root, const std::string& path = std::string() );
  static bool get_value( double&      value, js_node_t* root, const std::string& path = std::string() );
  static js_node_t* create( sim_t* sim, const std::string& input );
  static js_node_t* create( sim_t* sim, FILE* input );
  static void print( js_node_t* root, FILE* f=0, int spacing=0 );
  static const char* get_name( js_node_t* root );
};

// Handy Actions ============================================================

struct wait_action_base_t : public action_t
{
  wait_action_base_t( player_t* player, const char* name ) :
    action_t( ACTION_OTHER, name, player )
  { trigger_gcd = timespan_t::zero(); }

  virtual void execute()
  { player -> iteration_waiting_time += time_to_execute; }
};

// Wait For Cooldown Action =================================================

struct wait_for_cooldown_t : public wait_action_base_t
{
  cooldown_t* wait_cd;
  action_t* a;
  wait_for_cooldown_t( player_t* player, const char* cd_name );
  virtual bool usable_moving() { return a -> usable_moving(); }
  virtual timespan_t execute_time() const;
};

// actor_pair_t inlines

inline actor_pair_t::actor_pair_t( targetdata_t* td )
  : target( td->target ), source( td->source )
{}

// buff_t inlines

inline buff_t* buff_t::find( sim_t* s, const std::string& name ) { return find( s -> buff_list, name ); }
inline buff_t* buff_t::find( player_t* p, const std::string& name ) { return find( p -> buff_list, name ); }

// sim_t inlines

inline double sim_t::real() const                { return default_rng_ -> real(); }
inline bool   sim_t::roll( double chance ) const { return default_rng_ -> roll( chance ); }

#ifdef WHAT_IF

#define AOE_CAP 10

struct ticker_t // replacement for dot_t, handles any ticking buff/debuff
{
  dot_t stuff
  double crit;
  double haste;
  double mastery;
};

struct sim_t
{
  ...
  actor_t* actor_list;
  std::vector<player_t*> player_list;
  std::vector<mob_t*> mob_list;
  ...
  int get_aura_slot( const std::string& n, actor_t* source );
  ...
};

struct actor_t
{
  items, stats, action list, resource management...
  actor_t( const std::string& n, int type ) {}
  virtual actor_t*  choose_target();
  virtual void      execute_action() { choose_target(); execute_first_ready_action(); }
  virtual debuff_t* get_debuff( int aura_slot );
  virtual ticker_t* get_ticker( int aura_slot );
  virtual bool      is_ally( actor_t* other );
};

struct player_t : public actor_t
{
  scaling, current_target ( actor ), pet_list ...
  player_t( const std::string& n, int type ) : actor_t( n, type ) { sim.player_list.push_back( this ); }
};

struct pet_t : public actor_t
{
  owner, summon, dismiss...
  pet_t( const std::string& n, int type ) : actor_t( n, type ) {}
};

struct enemy_t : public actor_t
{
  health_by_time, health_per_player, arrise_at_time, arise_at_percent, ...
  enemy_t( const std::string& n ) : actor_t( n, ACTOR_ENEMY ) { sim.enemy_list.push_back( this ); }
};

struct action_t
{
  actor_t, ...
  action_t( const std::string& n );
  virtual int execute();
  virtual void schedule_execute();
  virtual double execute_time();
  virtual double haste();
  virtual double gcd();
  virtual bool ready();
  virtual void cancel();
  virtual void reset();
};

struct result_t
{
  actor_t* target;
  int type;
  bool hit;  // convenience
  bool crit; // convenience, two-roll (blocked crits, etc)
  double amount;
  ticker_t ticker;
};

struct ability_t : public action_t
{
  spell_data, resource, weapon, two_roll, self_cast, aoe, base_xyz (no player_xyz or target_xyz),
  direct_sp_coeff, direct_ap_coeff, tick_sp_coeff, tick_ap_coeff,
  harmful, healing, callbacks, std::vector<result_t> results,
  NO binary, NO repeating, NO direct_dmg, NO tick_dmg
  ability_t( spell_data_t* s ) : action_t( s -> name() ) {}
  virtual void  execute()
  {
    actor_t* targets[ AOE_CAP ];
    int num_targets = area_of_effect( targets );
    results.resize( num_targets );
    for ( int i=0; i < num_targets; i++ )
    {
      calculate_result( results[ i ], targets[ i ] );
      // "result" callbacks
    }
    consume_resource();
    for ( int i=0; i < num_targets; i++ )
    {
      schedule_travel( results[ i ] );
    }
    update_ready();
    // "cast" callbacks
  }
  virtual void impact( result_t& result )
  {
    if ( result.hit )
    {
      if ( result.amount > 0 )
      {
        if ( harmful )
        {
          assess_damage( result );
        }
        else if ( healing )
        {
          assess_healing( result );
        }
      }
      if ( num_ticks > 0 )
      {
        ticker_t* ticker = get_ticker( result.target );  // caches aura_slot
        ticker -> trigger( this, result.ticker );
        // ticker_t::trigger() handles dot work in existing action_t::impact()
      }
    }
    else
    {
      // miss msg
      stat -> add_result( result );
    }
  }
  virtual void   tick         ( ticker_t* );
  virtual void   last_tick    ( ticker_t* );
  virtual void   schedule_tick( ticker_t* );
  virtual int    calculate_num_ticks( double haste, double duration=0 );
  virtual double cost();
  virtual double haste();
  virtual bool   ready();
  virtual void   cancel();
  virtual void   reset();
  virtual void   consume_resource();
  virtual result_type_e calculate_attack_roll( actor_t* target );
  virtual result_type_e calculate_spell_roll( actor_t* target );
  virtual result_type_e calculate_roll( actor_t* target )
  {
    if ( weapon )
      return calculate_attack_roll( target );
    else
      return calculate_spell_roll( target );
  }
  virtual void calculate_result( result_t& result, actor_t* target )
  {
    result.type = calculate_roll( target );
    result.hit  = ( roll == ? );
    result.crit = ( roll == ? ) || ( two_roll );
    result.amount = calculate_direct_amount( target );
    if ( result.hit && num_ticks )
    {
      calculate_ticker( &( result.ticker ), target );
    }
    return result;
  }
  virtual void calculate_ticker( ticker_t* ticker, target )
  {
    if ( target ) ticker -> target = target;
    ticker -> amount  = calculate_tick_amount( ticker -> target );
    ticker -> crit    = calculate_crit_chance( ticker -> target );
    ticker -> haste   = calculate_haste      ( ticker -> target );
    ticker -> mastery = calculate_mastery    ( ticker -> target );
  }
  virtual void refresh_ticker( ticker_t* ticker )
  {
    calculate_ticker( ticker );
    ticker -> current_tick = 0;
    ticker -> added_ticks = 0;
    ticker -> added_time = 0;
    ticker -> num_ticks = calculate_num_ticks( ticker -> haste );
    ticker -> recalculate_finish();
  }
  virtual void extend_ticker_by_time( ticker_t* ticker, double extra_time )
  {
    int    full_tick_count   = ticker -> ticks() - 1;
    double full_tick_remains = ticker -> finish - ticker -> tick_event -> time;

    ticker -> haste = calculate_haste( ticker -> target );
    ticker -> num_ticks += calculate_num_ticks( ticker -> haste, full_tick_remains ) - full_tick_count;
    ticker -> recalculate_finish();
  }
  virtual void extend_ticker_by_ticks( ticker_t* ticker, double extra_ticks )
  {
    calculate_ticker( ticker );
    ticker -> added_ticks += extra_ticks;
    ticker -> num_ticks   += extra_ticks;
    ticker -> recalculate_finish();
  }
  virtual double calculate_weapon_amount( actor_t* target );
  virtual double calculate_direct_amount( actor_t* target );
  virtual double calculate_tick_amount  ( actor_t* target );
  virtual double calculate_power( actor_t* target )
  {
    return AP_multiplier * AP() + SP_multiplier * SP();
  }
  virtual double calculate_haste( actor_t* target )
  {
    if ( weapon )
      return calculate_attack_haste( target );
    else
      return calculate_spell_haste( target );
  }
  virtual double calculate_mastery( actor_t* target )
  {
    if ( weapon )
      return calculate_attack_mastery( target );
    else
      return calculate_spell_mastery( target );
  }
  virtual double calculate_miss_chance( actor_t* target )
  {
    if ( weapon )
      return calculate_attack_miss_chance( target );
    else
      return calculate_spell_miss_chance( target );
  }
  virtual double calculate_dodge_chance( actor_t* target )
  {
    if ( weapon )
      return calculate_attack_dodge_chance( target );
    else
      return 0;
  }
  virtual double calculate_parry_chance( actor_t* target )
  {
    if ( weapon )
      return calculate_attack_parry_chance( target );
    else
      return 0;
  }
  virtual double calculate_glance_chance( actor_t* target )
  {
    if ( weapon && auto_attack )
      return calculate_attack_glance_chance( target );
    else
      return 0;
  }
  virtual double calculate_block_chance( actor_t* target )
  {
    if ( weapon )
      return calculate_attack_block_chance( target );
    else
      return 0;
  }
  virtual double calculate_crit_chance( actor_t* target )
  {
    if ( weapon )
      return calculate_attack_crit_chance( target );
    else
      return calculate_spell_crit_chance( target );
  }
  virtual double calculate_crit_chance( ticker_t* ticker );
  virtual double calculate_source_multiplier();
  virtual double calculate_direct_multiplier() { return calculate_source_multiplier(); } // include crit bonus here
  virtual double calculate_tick_multiplier  () { return calculate_source_multiplier(); }
  virtual double calculate_target_multiplier( actor_t* target );
  virtual int       area_of_effect( actor_t* targets[] ) { targets[ 0 ] = self_cast ? actor : actor -> current_target; return 1; }
  virtual result_t& result(); // returns 0th "result", asserts if aoe
  virtual double    travel_time( actor_t* target );
  virtual void      schedule_travel( result_t& result );
  virtual void assess_damage( result_t& result )
  {
    result.amount *= calculate_target_multiplier( result.target ); // allows for action-specific target multipliers
    result.target -> assess_damage( result ); // adjust result as needed for flexibility
    stat -> add_result( result );
  }
  virtual void assess_healing( result_t& result )
  {
    result.amount *= calculate_target_multiplier( result.target ); // allows for action-specific target multipliers
    result.target -> assess_healing( result ); // adjust result as needed for flexibility
    stat -> add_result( result );
  }
};

#endif

#endif // SIMULATIONCRAFT_H
