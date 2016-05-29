module app {

  'use strict';

  /**
   * Settings service: read Settings from local storage
   * set new values, save to local storage
   */
  export class SettingsService {

    private logger: ILogger;

    constructor(logger: LoggerService,
                private $window: ng.IWindowService,
                private config: IApplicationConfig,
                private gettextCatalog: angular.gettext.gettextCatalog,
                private $locale: ng.ILocaleService ) {
      this.logger = logger.getLogger('settingsService');
    }
    /**
     * Utility method to set the language in the tools requiring it.
     * The current language is saved to the local storage.
     * If no parameter is specified, the language is loaded from local storage (if possible).
     * @param {string=} language The IETF language tag.
     */
    setLanguage(language?: string) {
        this.logger.log(language);
      language = language || this.$window.localStorage.getItem('language');
      let isSupportedLanguage = _.includes(this.config.supportedLanguages, language);

      // If no exact match is found, search without the region
      if (!isSupportedLanguage && language) {
        let languagePart = language.split('_')[0];
        language = _.find(this.config.supportedLanguages,
          (supportedLanguage: string) => _.startsWith(supportedLanguage, languagePart));
        isSupportedLanguage = !!language;
      }

      // Fallback if language is not supported
      if (!isSupportedLanguage) {
        language = 'en_US';
      }

      // Configure translation with gettext
      this.gettextCatalog.setCurrentLanguage(language);
      this.$locale.id = language;
      this.$window.localStorage.setItem('language', language);
    };



  }

  angular
    .module('app')
    .service('settingsService', SettingsService);

}
