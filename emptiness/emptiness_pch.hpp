#pragma once

#include <gl/glew.h>
#pragma comment(lib,"opengl32.lib")
#include <SFML/Window.hpp>

#include <gsl.h>
#include <concurrent_queue.h>
#include <boost/timer/timer.hpp>
#include <boost/range/algorithm/fill.hpp>
#include <boost/range/iterator_range.hpp>

#include <iostream>
#include <future>
#include <thread>

#include "folder_monitor.hpp"
#include "gl_utils.hpp"