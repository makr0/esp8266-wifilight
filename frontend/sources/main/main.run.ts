module app {

  'use strict';

  /**
   * Entry point of the application.
   * Initializes application and root controller.
   */
  function main($window: ng.IWindowService,
                $locale: ng.ILocaleService,
                $rootScope: any,
                $state: angular.ui.IStateService,
                $timeout: ng.ITimeoutService,
                $cordovaKeyboard: any,
                gettextCatalog: angular.gettext.gettextCatalog,
                _: _.LoDashStatic,
                config: IApplicationConfig,
                logger: LoggerService,
                $mdSidenav: any,
                restService: RestService,
                settingsService: SettingsService,
                esplightService: EsplightService ) {

    /*
     * Root view model
     */

    let vm = $rootScope;

    vm.pageTitle = '';
    vm.viewTitle = 'initial';

    vm.toggleLeftNav = function() {
      $mdSidenav('leftNav').toggle();
    };
    vm.toggleRightNav = function() {
      $mdSidenav('rightNav').toggle();
    };
    vm.closeNavs = function() {
      $mdSidenav('leftNav').close();
      $mdSidenav('rightNav').close();
    };
    vm.lightServer = 'http://192.168.76.109';

    vm.saveLightUrl = function() {
      console.log( vm.lightServer);
      esplightService.setServer(vm.lightServer);
    };

    /**
     * Updates title on view change.
     */
    vm.$on('$stateChangeSuccess', (event: any, toState: angular.ui.IState) => {
      updateTitle(toState.data ? toState.data.title : null);
      vm.closeNavs();
    });

    /**
     * Updates title on language change.
     */
    vm.$on('gettextLanguageChanged', () => {
      updateTitle($state.current.data ? $state.current.data.title : null);
    });

    init();

    /*
     * Internal
     */

    /**
     * Initializes the root controller.
     */
    function init() {
      let _logger: ILogger = logger.getLogger('main');
      // Enable debug mode for translations
      gettextCatalog.debug = config.environment.debug;

      settingsService.setLanguage();

      // Set REST server configuration
      restService.setServer(config.environment.server);
      esplightService.setServer(vm.lightServer);

      // Cordova platform and plugins init
      $window.document.addEventListener('deviceready', () => {

        // Hide splash screen
        let splashScreen = $window.navigator.splashscreen;
        if (splashScreen) {
          $timeout(() => {
            splashScreen.hide();
          }, 100);
        }

        // Detect and set default language
        let globalization = $window.navigator.globalization;
        if (globalization) {
          // Use cordova plugin to retrieve device's locale
          globalization.getPreferredLanguage((language) => {
            _logger.log('Setting device locale "' + language.value + '" as default language');
            vm.$apply(() => {
              vm.setLanguage(language.value);
            });
          }, null);
        }

        if ($window.cordova && $window.cordova.plugins.Keyboard) {
          $cordovaKeyboard.disableScroll(true);
        }

      }, false);
    }

    /**
     * Updates the title.
     * @param {?string=} stateTitle Title of current state, to be translated.
     */
    function updateTitle(stateTitle?: string) {
      vm.pageTitle = gettextCatalog.getString('APP_NAME');

      if (stateTitle) {
        vm.viewTitle = gettextCatalog.getString(stateTitle);
        vm.pageTitle += ' | ' + vm.viewTitle;
      }
    }

  }

  angular
    .module('app')
    .run(main);

}
