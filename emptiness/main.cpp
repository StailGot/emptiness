#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <memory>
#include <type_traits>
#include <tuple>
#include <stack>


namespace btree{ namespace lcrs {

struct node_t
{
  using value_type = int;

  using node_ptr   = std::unique_ptr<node_t>;
  using siblings   = std::vector<node_ptr>;

  node_ptr   _child    = {};
  siblings   _siblings = {};
  value_type _value    = {};

  explicit node_t( const value_type & value ) 
    : _value( value )
  {}

  node_t() = default;
};


inline void add_child( node_t::node_ptr & n, const node_t::value_type & value )
{
  node_t::node_ptr * child = &n->_child;

  while ( child != nullptr && *child )
  {
    child = &(*child)->_child;
  }

  *child = std::make_unique<node_t>(value);
}


inline void add_sibling( node_t::node_ptr & n, const node_t::value_type & value )
{
  n->_siblings.emplace_back( std::make_unique<node_t>(value) );
}


template< typename Fn >
void for_each( node_t::node_ptr & root, const Fn & fn )
{
  fn( root );

  if ( root->_child )
    for_each( root->_child, fn );

  for ( auto && sibling : root->_siblings )
    if ( sibling )
      for_each( sibling, fn );
}

using level_t = std::tuple<node_t::node_ptr*, node_t::node_ptr*, size_t>;


namespace detail{

inline bool is_valid_level( const level_t & level )
{
  return !!(std::get<0>(level) ? std::get<0>(level) : std::get<1>(level)) ;
}


inline node_t::node_ptr * get_node_from_level( level_t & level )
{
  return std::get<0>(level);
}


inline bool next_sibling( std::stack<level_t> & stack, level_t & level )
{
  bool result = false;

  node_t::node_ptr * next;
  node_t::node_ptr * next_top;
  size_t index;
  std::tie( next, next_top, index ) = level;
  //auto [next, next_top, index] = level; // c++17

  if ( next_top && *next_top && index < (*next_top)->_siblings.size() )
  {
    node_t::node_ptr * node = &(*next_top)->_siblings[index++];
    if ( node && *node && !(*node)->_siblings.empty() )
      stack.emplace( /*next*/ nullptr, node, 0 );

    next = node;
    level = std::tie( next, next_top, index );
   
    result = true;
  }

  return result;
}

__forceinline bool next_child( std::stack<level_t> & stack, level_t & level )
{
  bool result = false;

  node_t::node_ptr * next;
  node_t::node_ptr * next_top;
  size_t index;
  std::tie( next, next_top, index ) = level;
  //auto [next, next_top, index] = level; // c++17

  if ( next && *next && (*next)->_child )
  {
    if ((*next)->_siblings.size())
      stack.emplace( /*next*/ nullptr, next_top, index );
  
    next_top = next = &(*next)->_child;
    level = std::tie( next, next_top, index );
  
    result = true;
  }

  return result;
}

} // namespace detail



inline level_t & get_next_node( level_t & level, std::stack<level_t> & stack )
{
  if ( detail::is_valid_level(level) || !stack.empty() )
  {
    if ( detail::next_child(stack, level) )
    {
      // empty
    }
    else if ( detail::next_sibling(stack, level) )
    {
      // empty
    }
    else if ( !stack.empty() )
    {
      level = stack.top();
      stack.pop();

      detail::next_sibling( stack, level );
    }
    else
    {
      // end
      level = {};
    }
  }

  return level;
}


namespace iterator
{
  struct tree_iterator
  {
    
  };
} // namespace iterator


}} // namespace btree::lcrs


int main()
{
  using btree::lcrs::add_sibling;
  using btree::lcrs::add_sibling;

  btree::lcrs::node_t::node_ptr start = std::make_unique<btree::lcrs::node_t>();

  add_child( start, 10 );
  add_child( start, 20 );
  add_child( start, 30 );

  add_sibling( start, 101 );
  add_sibling( start, 102 );
  add_sibling( start, 103 );

  add_sibling( start->_child, 201 );
  add_sibling( start->_child, 202 );
  add_child  ( start->_child, 212 );

  add_sibling( start->_child->_child, 301 );
  add_sibling( start->_child->_child, 302 );
  add_sibling( start->_child->_child, 303 );

  add_sibling( start->_child->_child->_siblings[1], 313 );
  add_child  ( start->_child->_child->_siblings[1], 3134 );

  size_t count = 0;

  auto traverse_function = 
    [&count]( const btree::lcrs::node_t::node_ptr & node )
  {
    ++count;
    std::cout << node->_value << "\n";
  };

  for_each( start, traverse_function );
  std::cout << "count: " << count << "\n" << "\n";


  count = 0;
  std::stack< btree::lcrs::level_t > stack;

  btree::lcrs::level_t state{ &start, &start, 0};

  while (btree::lcrs::detail::is_valid_level(state) )
  {
    auto && node = btree::lcrs::detail::get_node_from_level(state);
    //if( node && *node )
      traverse_function( *node );

    //count ++;
    state = btree::lcrs::get_next_node( state, stack );
  }

  std::cout << "count: " << count << "\n" << "\n";
}