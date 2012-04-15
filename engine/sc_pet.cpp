// ==========================================================================
// Dedmonwakeen's Raid DPS/TPS Simulator.
// Send questions to natehieter@gmail.com
// ==========================================================================

#include "simulationcraft.hpp"

// ==========================================================================
// Pet
// ==========================================================================

// pet_t::pet_t =============================================================

void pet_t::init_pet_t_()
{
  target = owner -> target;
  level = owner -> level;
  full_name_str = owner -> name_str + '_' + name_str;
  expiration = 0;

  owner -> pet_list.push_back( this );

  // Pets have inherent 5% critical strike chance if not overridden.
  base_spell_crit  = 0.05;
  base_attack_crit = 0.05;

  stamina_per_owner = 0.75;
  intellect_per_owner = 0.30;

  party = owner -> party;

  // Inherit owner's dbc state
  dbc.ptr = owner -> dbc.ptr;
}
pet_t::pet_t( sim_t*             s,
              player_t*          o,
              const std::string& n,
              bool               g ) :
  player_t( s, g ? PLAYER_GUARDIAN : PLAYER_PET, n ),
  owner( o ), summoned( false ), pet_type( PET_NONE )
{
  init_pet_t_();
}

pet_t::pet_t( sim_t*             s,
              player_t*          o,
              const std::string& n,
              pet_type_e         pt,
              bool               g ) :
  player_t( s, pt == PET_ENEMY ? ENEMY_ADD : g ? PLAYER_GUARDIAN : PLAYER_PET, n ),
  owner( o ), summoned( false ), pet_type( pt )
{
  init_pet_t_();
}

// player_t::pet_attribute =================================

double pet_t::composite_attribute( attribute_type_e attr ) const
{
  double a = attribute[ attr ];

  switch ( attr )
  {
  case ATTR_INTELLECT:
    a += intellect_per_owner * owner -> intellect();
    break;
  case ATTR_STAMINA:
    a += stamina_per_owner * owner -> stamina();
    break;
  default:
    break;
  }

  return a;
}

// pet_t::init_base =========================================================

void pet_t::init_base()
{}

// pet_t::init_target =======================================================

void pet_t::init_target()
{
  if ( ! target_str.empty() )
    player_t::init_target();
  else
    target = owner -> target;
}

void pet_t::init_talents()
{
  spec = primary_tree();
}
// pet_t::reset =============================================================

void pet_t::reset()
{
  player_t::reset();

  expiration = 0;
}

// pet_t::summon ============================================================

void pet_t::summon( timespan_t duration )
{
  if ( sim -> log )
  {
    log_t::output( sim, "%s summons %s. for %.2fs", owner -> name(), name(), duration.total_seconds() );
  }

  distance = owner -> distance;

  owner -> active_pets++;

  summoned = true;

  // Take care of remaining expiration
  if ( expiration )
  {
    event_t::cancel( expiration );
    expiration = 0;
  }

  if ( duration > timespan_t::zero() )
  {
    struct expiration_t : public event_t
    {
      expiration_t( sim_t* sim, pet_t* p, timespan_t duration ) :
        event_t( sim, p )
      {
        sim -> add_event( this, duration );
      }

      virtual void execute()
      {
        player -> cast_pet() -> expiration = 0;
        if ( ! player -> sleeping ) player -> cast_pet() -> dismiss();
      }
    };
    expiration = new ( sim ) expiration_t( sim, this, duration );
  }

  arise();
}

// pet_t::dismiss ===========================================================

void pet_t::dismiss()
{
  if ( sim -> log ) log_t::output( sim, "%s dismisses %s", owner -> name(), name() );

  owner -> active_pets--;

  if ( expiration )
  {
    event_t::cancel( expiration );
    expiration = 0;
  }

  demise();
}

// pet_t::assess_damage =====================================================

double pet_t::assess_damage( double              amount,
                             const school_type_e school,
                             dmg_type_e          type,
                             result_type_e       result,
                             action_t*           action )
{
  if ( ! action || action -> aoe )
    amount *= 0.10;

  return player_t::assess_damage( amount, school, type, result, action );
}

// pet_t::combat_begin ======================================================

void pet_t::combat_begin()
{
  // By default, only report statistics in the context of the owner
  quiet = ! sim -> report_pets_separately;

  player_t::combat_begin();
}

// pet_t::composite_player_multiplier ======================================================

double pet_t::composite_player_multiplier( const school_type_e school, const action_t* a ) const
{
  double m = player_t::composite_player_multiplier( school, a );

  // FIXME: check if Guardian Pets benefit from it or not.
  if ( owner -> race == RACE_ORC )
  {
    m  *= 1.05;
  }

  return m;
}

// pet_t::find_pet_spell =============================================

const spell_data_t* pet_t::find_pet_spell( const std::string& name, const std::string& token )
{
  unsigned spell_id = dbc.pet_ability_id( type, name.c_str() );

  if ( ! spell_id || ! dbc.spell( spell_id ) )
  {
    if ( ! owner )
      return spell_data_t::not_found();

    return owner -> find_pet_spell( name, token );
  }

  dbc_t::add_token( spell_id, token, dbc.ptr );

  return ( dbc.spell( spell_id ) );
}
