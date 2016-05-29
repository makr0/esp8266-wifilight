module app {

  'use strict';

  export class SettingsController {

    currentLocale: ng.ILocaleService;
    private logger: ILogger;
    private languages: Array<String>;

    constructor( logger: LoggerService,
                $locale: ng.ILocaleService,
                private config: IApplicationConfig,
                private settingsService: SettingsService) {

      this.logger = logger.getLogger('settings');
      this.currentLocale = $locale;
      this.logger.log('init');
      this.languages = config.supportedLanguages;
      console.log(this.currentLocale);
    }

    setLanguage(language: string) {
        this.logger.log(language);
        this.settingsService.setLanguage(language);
    }

  }

  angular
    .module('app')
    .controller('settingsController', SettingsController);

}
