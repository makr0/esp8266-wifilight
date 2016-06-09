module app {

  'use strict';

  export class SettingsController {

    currentLocale: ng.ILocaleService;
    lightServer: string;
    private logger: ILogger;
    private languages: Array<String>;
    private foundDevices: any;

    constructor( logger: LoggerService,
                $locale: ng.ILocaleService,
                esplightService: EsplightService,
                private $scope: ng.IScope,
                private $window: Window,
                private config: IApplicationConfig,
                private settingsService: SettingsService) {

      this.logger = logger.getLogger('settings');
      this.currentLocale = $locale;
      this.logger.log('init');
      this.languages = config.supportedLanguages;
      this.lightServer = '192.168.76.109';
      console.log(this.currentLocale);

      $scope.$watch(() => this.lightServer, (newValue, oldValue) => {
        if (oldValue !== newValue) {
          if ( !angular.isUndefined( newValue ) ) {
            esplightService.setServer(newValue);
          }
        }
      }, true);
    }

    setLanguage(language: string) {
        this.logger.log(language);
        this.settingsService.setLanguage(language);
    }

    scanDevices() {
        console.log('scan starts');
        this.$window.serviceDiscovery.getNetworkServices('ssdp:wifilight', true,
            (devices) => {
              console.log('scan complete');
              console.log(devices);
              this.$scope.$apply(() => { this.foundDevices = devices; });
            },
            () => this.logger.log('fail')
        );
    }

  }

  angular
    .module('app')
    .controller('settingsController', SettingsController);

}
