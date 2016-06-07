class configStore
{
    private:
        bool shouldSaveConfig;
        bool fs_mounted;
        void do_saveAll();

    public:
        configStore();

        void handle();
        void save();
        char *getValue(String key);
};