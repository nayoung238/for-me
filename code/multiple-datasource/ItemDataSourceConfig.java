package com.example.datasourcetest.zconfig;

import com.zaxxer.hikari.HikariDataSource;
import jakarta.persistence.EntityManagerFactory;
import org.hibernate.cfg.AvailableSettings;
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.beans.factory.config.ConfigurableListableBeanFactory;
import org.springframework.boot.autoconfigure.jdbc.DataSourceProperties;
import org.springframework.boot.context.properties.ConfigurationProperties;
import org.springframework.boot.orm.jpa.EntityManagerFactoryBuilder;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.context.annotation.Primary;
import org.springframework.data.jpa.repository.config.EnableJpaRepositories;
import org.springframework.orm.hibernate5.SpringBeanContainer;
import org.springframework.orm.jpa.JpaTransactionManager;
import org.springframework.orm.jpa.LocalContainerEntityManagerFactoryBean;
import org.springframework.transaction.PlatformTransactionManager;
import org.springframework.transaction.annotation.EnableTransactionManagement;

import javax.sql.DataSource;
import java.util.HashMap;
import java.util.Map;
import java.util.Properties;

@Configuration
@EnableTransactionManagement
@EnableJpaRepositories(
        entityManagerFactoryRef = "item-entity-manager-factory",
        transactionManagerRef = "item-transaction-manager",
        basePackages = "com.example.datasourcetest.item.repo"
)
public class ItemDataSourceConfig {

    @Bean(name = "item-datasource-properties")
    @ConfigurationProperties("spring.item-datasource")
    public DataSourceProperties dataSourceProperties() {
        return new DataSourceProperties();
    }

    @Primary
    @Bean(name = "item-hikari-data-source")
    @ConfigurationProperties(prefix = "spring.item-datasource.hikari")
    public HikariDataSource hikariDataSource(
            @Qualifier("item-datasource-properties") DataSourceProperties dataSourceProperties)
    {
        return dataSourceProperties
                .initializeDataSourceBuilder()
                .type(HikariDataSource.class)
                .build();
    }

    @Primary
    @Bean(name = "item-entity-manager-factory")
    public LocalContainerEntityManagerFactoryBean entityManagerFactory(
            EntityManagerFactoryBuilder builder,
            ConfigurableListableBeanFactory beanFactory,
            @Qualifier("item-hikari-data-source") DataSource dataSource
    ) {
//        LocalContainerEntityManagerFactoryBean entityManagerFactoryBean = new LocalContainerEntityManagerFactoryBean();
//        entityManagerFactoryBean.setDataSource(dataSource);
//        entityManagerFactoryBean.setPackagesToScan("com.example.datasourcetest.item");
//        entityManagerFactoryBean.setPersistenceProvider(new HibernatePersistenceProvider());
//        entityManagerFactoryBean.setJpaProperties(getJpaProperties());
//        entityManagerFactoryBean.setJpaVendorAdapter(new HibernateJpaVendorAdapter());
//        return entityManagerFactoryBean;

        LocalContainerEntityManagerFactoryBean entityManagerFactoryBean = builder
                .dataSource(dataSource)
                .packages("com.example.datasourcetest.item")
                .persistenceUnit("1")
                .properties(getProperties())
                .build();

        entityManagerFactoryBean.getJpaPropertyMap()
                .put(AvailableSettings.BEAN_CONTAINER, new SpringBeanContainer(beanFactory));
        return entityManagerFactoryBean;
    }

    private Properties getJpaProperties() {
        Properties properties = new Properties();
        properties.setProperty("hibernate.dialect", "org.hibernate.dialect.MySQLDialect");
        properties.setProperty("hibernate.hbm2ddl.auto", "create-drop");
        return properties;
    }

    private Map<String, Object> getProperties() {
        Map<String, Object> properties = new HashMap<>();
        properties.put("hibernate.dialect", "org.hibernate.dialect.MySQLDialect");
        properties.put("hibernate.hbm2ddl.auto", "create-drop");
        properties.put("hibernate.show_sql", "true");
        return properties;
    }

    @Primary
    @Bean(name = "item-transaction-manager")
    public PlatformTransactionManager transactionManager(
            @Qualifier("item-entity-manager-factory") EntityManagerFactory entityManagerFactory
    ) {
        return new JpaTransactionManager(entityManagerFactory);
    }
}

