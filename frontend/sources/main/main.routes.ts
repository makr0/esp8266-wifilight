module app {

  'use strict';

  /**
   * Configures the application routes.
   */
  function routeConfig($stateProvider: angular.ui.IStateProvider,
                       $urlRouterProvider: angular.ui.IUrlRouterProvider,
                       gettext: angular.gettext.gettextFunction) {

    // Routes configuration
    $urlRouterProvider.otherwise('/');

    $stateProvider
      .state('app', {
        views: {
          'shell': {
            templateUrl: 'modules/shell/shell.html',
            controller: 'shellController as shell'
          }
        }
      })
      .state('app.home', {
        url: '/',
        views: {
          'main': {
            templateUrl: 'modules/screens/home/home.html',
            controller: 'homeController as vm',
          },
        },
        data: {title: gettext('Home')}
      })
      .state('app.effects', {
        url: '/effects',
        views: {
          'main': {
            templateUrl: 'modules/screens/effects/effects.html',
            controller: 'effectsController as vm'
          }
        },
        data: {title: gettext('Effects')}
      })
      .state('app.about', {
        url: '/about',
        views: {
          'main': {
            templateUrl: 'modules/screens/about/about.html',
            controller: 'aboutController as vm'
          },
          'side': {
            templateUrl: 'modules/screens/home/home.html',
            controller: 'homeController as vm',
          }
        },
        data: {title: gettext('About')}
      })
      .state('app.settings', {
        url: '/settings',
        views: {
          'main': {

            templateUrl: 'modules/screens/settings/settings.html',
            controller: 'settingsController as vm'
          },
          'side': {
            templateUrl: 'modules/screens/home/home.html',
            controller: 'homeController as vm',
          }
        },
        data: {title: gettext('Settings')}
      });

  }

  angular
    .module('app')
    .config(routeConfig);

}

